/*=========================================================================
*
*  Copyright Insight Software Consortium
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*         http://www.apache.org/licenses/LICENSE-2.0.txt
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
*=========================================================================*/
//  Software Guide : BeginCommandLineArgs
//    INPUTS: {BrainProtonDensitySlice.png}
//    ARGUMENTS:   OtsuMultipleThresholdsOutput png 4
//  Software Guide : EndCommandLineArgs
// Software Guide : BeginLatex
//
// This example illustrates how to use the \doxygen{OtsuMultipleThresholdsCalculator}.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
#include "itkOtsuMultipleThresholdsCalculator.h"
// Software Guide : EndCodeSnippet
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkScalarImageToHistogramGenerator.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkNumericTraits.h"
#include <iomanip>
#include "MedidasDiversidadeFuncional.h"
#include <cstdio>
int main(int argc, char * argv[])
{
	
	//Convenience type alias
	using InputPixelType = unsigned short;
	using OutputPixelType = unsigned char;
	using InputImageType = itk::Image< InputPixelType, 3 >;
	using OutputImageType = itk::Image< OutputPixelType, 3 >;
	// Software Guide : BeginLatex
	//
	// \code{OtsuMultipleThresholdsCalculator} calculates thresholds for a given
	// histogram so as to maximize the between-class variance. We use
	// \code{ScalarImageToHistogramGenerator} to generate histograms. The histogram
	// type defined by the generator is then used to instantiate the type of the
	// Otsu threshold calculator.
	//
	// Software Guide : EndLatex
	// Software Guide : BeginCodeSnippet
	using ScalarImageToHistogramGeneratorType =
		itk::Statistics::ScalarImageToHistogramGenerator<InputImageType>;
	using HistogramType = ScalarImageToHistogramGeneratorType::HistogramType;
	using CalculatorType = itk::OtsuMultipleThresholdsCalculator<HistogramType>;
	// Software Guide : EndCodeSnippet
	using ReaderType = itk::ImageFileReader< InputImageType >;
	using WriterType = itk::ImageFileWriter< OutputImageType >;
	// Software Guide : BeginLatex
	//
	// Once thresholds are computed we will use \code{BinaryThresholdImageFilter}
	// to segment the input image.
	//
	// Software Guide : EndLatex
	// Software Guide : BeginCodeSnippet
	using FilterType = itk::BinaryThresholdImageFilter<
		InputImageType, OutputImageType >;
	// Software Guide : EndCodeSnippet
	// Software Guide : BeginLatex
	//
	// Create a histogram generator and calculator using the standard
	// \code{New()} method.
	//
	// Software Guide : EndLatex
	// Software Guide : BeginCodeSnippet
	ScalarImageToHistogramGeneratorType::Pointer scalarImageToHistogramGenerator
		= ScalarImageToHistogramGeneratorType::New();
	CalculatorType::Pointer calculator = CalculatorType::New();
	FilterType::Pointer filter = FilterType::New();
	// Software Guide : EndCodeSnippet
	ReaderType::Pointer reader = ReaderType::New();
	WriterType::Pointer writer = WriterType::New();
	reader->SetFileName("C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/_  8/imagemUsar.nrrd");
	// Software Guide : BeginLatex
	//
	// Set the following properties for the histogram generator and the
	// calculators, in this case grabbing the number of thresholds from
	// the command line.
	//
	// Software Guide : EndLatex
	// Software Guide : BeginCodeSnippet
	MedidasDiversidadeFuncional md;
	//float minimo = md.getMinimum1(reader->GetOutput);
	
	scalarImageToHistogramGenerator->SetNumberOfBins(601);
	calculator->SetNumberOfThresholds(1);
	// Software Guide : EndCodeSnippet
	constexpr OutputPixelType outsideValue = 1500 ;
	constexpr OutputPixelType insideValue = 900;
	filter->SetOutsideValue(outsideValue);
	filter->SetInsideValue(insideValue);
	//Connect Pipeline
	
	ImageType3D::SpacingType spacing;
	// Units (e.g., mm, inches, etc.) are defined by the application.
	spacing[0] = 0.33; // spacing along X
	spacing[1] = 0.33; // spacing along Y
	spacing[2] = 1.20; // spacing along Z
					   // Software Guide : EndCodeSnippet

	//ImageType3D::Pointer imagemSintetica = md.createImageSintetica(spacing);

	
	// Software Guide : BeginLatex
	//
	// The pipeline will look as follows:
	//
	// Software Guide : EndLatex
	// Software Guide : BeginCodeSnippet
	scalarImageToHistogramGenerator->SetInput(reader->GetOutput());
	calculator->SetInputHistogram(
		scalarImageToHistogramGenerator->GetOutput());
	filter->SetInput(reader->GetOutput());
	writer->SetInput(filter->GetOutput());
	// Software Guide : EndCodeSnippet
	//Invoke pipeline
	try
	{
		reader->Update();
	}
	catch (itk::ExceptionObject & excp)
	{
		std::cerr << "Exception thrown while reading image" << excp << std::endl;
	}
	scalarImageToHistogramGenerator->Compute();
	try
	{
		calculator->Compute();
	}
	catch (itk::ExceptionObject & excp)
	{
		std::cerr << "Exception thrown " << excp << std::endl;
	}
	// Software Guide : BeginLatex
	//
	// Here we obtain a \code{const} reference to the thresholds by calling
	// the \code{GetOutput()} method.
	// \index{itk::OtsuMultipleThresholdsCalculator!GetOutput()}
	//
	// Software Guide : EndLatex
	//Get Thresholds
	// Software Guide : BeginCodeSnippet
	const CalculatorType::OutputType &thresholdVector = calculator->GetOutput();
	// Software Guide : EndCodeSnippet
	// Software Guide : BeginLatex
	//
	// We now iterate through \code{thresholdVector}, printing each value to the
	// console and writing an image thresholded with adjacent values from the
	// container.  (In the edge cases, the minimum and maximum values of the
	// \code{InternalPixelType} are used).
	//
	// Software Guide : EndLatex
	std::string outputFileBase = "C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/_  8/";
	InputPixelType lowerThreshold = itk::NumericTraits<InputPixelType>::min();
	InputPixelType upperThreshold;
	// Software Guide : BeginCodeSnippet
	for (auto itNum = thresholdVector.begin();
		itNum != thresholdVector.end();
		++itNum)
	{
		std::cout << "OtsuThreshold["
			<< (int)(itNum - thresholdVector.begin())
			<< "] = "
			<< static_cast<itk::NumericTraits<
			CalculatorType::MeasurementType>::PrintType>(*itNum)
			<< std::endl;
		// Software Guide : EndCodeSnippet
		upperThreshold = static_cast<InputPixelType>(*itNum);
		int valor = upperThreshold; 
		cout << "\nValor do limiar: " << endl;
		filter->SetLowerThreshold(lowerThreshold);
		filter->SetUpperThreshold(upperThreshold);
		lowerThreshold = upperThreshold;
		std::ostringstream outputFilename;
		outputFilename << outputFileBase
			<< std::setfill('0') << std::setw(3) << (itNum - thresholdVector.begin())
			<< "."
			<< "C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/_  8/teste.nrrd";
		writer->SetFileName(outputFilename.str());
		try
		{
			writer->Update();
		}
		catch (itk::ExceptionObject & excp)
		{
			std::cerr << "Exception thrown " << excp << std::endl;
		}
	}
	// Software Guide : BeginLatex
	//
	// Also write out the image thresholded between the upper threshold and
	// the max intensity.
	//
	// Software Guide : EndLatex
	// Software Guide : BeginCodeSnippet
	upperThreshold = itk::NumericTraits<InputPixelType>::max();
	filter->SetLowerThreshold(lowerThreshold);
	filter->SetUpperThreshold(upperThreshold);
	// Software Guide : EndCodeSnippet
	std::ostringstream outputFilename2;
	outputFilename2 << outputFileBase
		<< std::setfill('0') << std::setw(3) << thresholdVector.size()
		<< "."
		<< "C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/_  8/";
	writer->SetFileName(outputFilename2.str());
	try
	{
		writer->Update();
	}
	catch (itk::ExceptionObject & excp)
	{
		std::cerr << "Exception thrown " << excp << std::endl;
	}
	return EXIT_SUCCESS;
}