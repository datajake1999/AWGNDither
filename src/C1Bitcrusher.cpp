#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#ifndef __C1Bitcrusher__
#include "C1Bitcrusher.h"
#endif

C1Bitcrusher::C1Bitcrusher (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 0, kNumParams)
{
	// init
	setNumInputs (2);	// stereo input
	setNumOutputs (2);	// stereo output
	Disable = 0;
	BitDepth = 16;
	DCBias = 0;
	Dither = 1;
	DitherType = 1;
	DitherInError = 1;
	NoiseShaping = 1;
	NoiseShapingMode = 1;
	Quantize = 1;
	QuantizationMode = 1;
	OnlyError = 0;
	AutoDither = 0;
	InvertDither = 0;
	Seed = 1;
	SeedWithTime = 0;
	ClipPreQuantization = 0;
	ClipValue = 1;
	ClipPostQuantization = 1;
	InGain = 1;
	OutGain = 1;
	DitherGain = 1;
	NoiseShapingGain = 1;
	NumAmplitudes = powf(2, BitDepth);
	resume ();
}

void C1Bitcrusher::resume ()
{
	if (SeedWithTime >= 0.5)
	{
		srand(time(0));
	}
	else
	{
		srand((int)Seed);
	}
	quantized[0] = 0;
	quantized[1] = 0;
	error[0] = 0;
	error[1] = 0;
	AudioEffectX::resume();
}

void C1Bitcrusher::setParameter (VstInt32 index, float value)
{
	if (index == kDisable)
	{
		Disable = value;
	}
	else if (index == kBitDepth)
	{
		BitDepth = value*32;
		if (BitDepth > 32) BitDepth = 32;
		else if (BitDepth < 1) BitDepth = 1;
		NumAmplitudes = powf(2, BitDepth);
	}
	else if (index == kDCBias)
	{
		DCBias = (value*4.0f)-2.0f;
	}
	else if (index == kDither)
	{
		Dither = value;
	}
	else if (index == kDitherType)
	{
		DitherType = value;
	}
	else if (index == kDitherInError)
	{
		DitherInError = value;
	}
	else if (index == kNoiseShaping)
	{
		NoiseShaping = value;
	}
	else if (index == kNoiseShapingMode)
	{
		NoiseShapingMode = value;
	}
	else if (index == kQuantize)
	{
		Quantize = value;
	}
	else if (index == kQuantizationMode)
	{
		QuantizationMode = value;
	}
	else if (index == kOnlyError)
	{
		OnlyError = value;
	}
	else if (index == kAutoDither)
	{
		AutoDither = value;
	}
	else if (index == kInvertDither)
	{
		InvertDither = value;
	}
	else if (index == kSeed)
	{
		Seed = value*1000;
		if (Seed < 1) Seed = 1;
	}
	else if (index == kSeedWithTime)
	{
		SeedWithTime = value;
	}
	else if (index == kClipPreQuantization)
	{
		ClipPreQuantization = value;
	}
	else if (index == kClipValue)
	{
		ClipValue = value;
	}
	else if (index == kClipPostQuantization)
	{
		ClipPostQuantization = value;
	}
	else if (index == kInGain)
	{
		InGain = value;
	}
	else if (index == kOutGain)
	{
		OutGain = value;
	}
	else if (index == kDitherGain)
	{
		DitherGain = value;
	}
	else if (index == kNoiseShapingGain)
	{
		NoiseShapingGain = value;
	}
}

void C1Bitcrusher::setParameterAutomated (VstInt32 index, float value)
{
	if (index == kDisable)
	{
		Disable = value;
	}
	else if (index == kBitDepth)
	{
		BitDepth = value*32;
		if (BitDepth > 32) BitDepth = 32;
		else if (BitDepth < 1) BitDepth = 1;
		NumAmplitudes = powf(2, BitDepth);
	}
	else if (index == kDCBias)
	{
		DCBias = (value*4.0f)-2.0f;
	}
	else if (index == kDither)
	{
		Dither = value;
	}
	else if (index == kDitherType)
	{
		DitherType = value;
	}
	else if (index == kDitherInError)
	{
		DitherInError = value;
	}
	else if (index == kNoiseShaping)
	{
		NoiseShaping = value;
	}
	else if (index == kNoiseShapingMode)
	{
		NoiseShapingMode = value;
	}
	else if (index == kQuantize)
	{
		Quantize = value;
	}
	else if (index == kQuantizationMode)
	{
		QuantizationMode = value;
	}
	else if (index == kOnlyError)
	{
		OnlyError = value;
	}
	else if (index == kAutoDither)
	{
		AutoDither = value;
	}
	else if (index == kInvertDither)
	{
		InvertDither = value;
	}
	else if (index == kSeed)
	{
		Seed = value*1000;
		if (Seed < 1) Seed = 1;
	}
	else if (index == kSeedWithTime)
	{
		SeedWithTime = value;
	}
	else if (index == kClipPreQuantization)
	{
		ClipPreQuantization = value;
	}
	else if (index == kClipValue)
	{
		ClipValue = value;
	}
	else if (index == kClipPostQuantization)
	{
		ClipPostQuantization = value;
	}
	else if (index == kInGain)
	{
		InGain = value;
	}
	else if (index == kOutGain)
	{
		OutGain = value;
	}
	else if (index == kDitherGain)
	{
		DitherGain = value;
	}
	else if (index == kNoiseShapingGain)
	{
		NoiseShapingGain = value;
	}
}

float C1Bitcrusher::getParameter (VstInt32 index)
{
	if (index == kDisable)
	{
		return Disable;
	}
	else if (index == kBitDepth)
	{
		return BitDepth/32;
	}
	else if (index == kDCBias)
	{
		return (DCBias+2.0f)/4.0f;
	}
	else if (index == kDither)
	{
		return Dither;
	}
	else if (index == kDitherType)
	{
		return DitherType;
	}
	else if (index == kDitherInError)
	{
		return DitherInError;
	}
	else if (index == kNoiseShaping)
	{
		return NoiseShaping;
	}
	else if (index == kNoiseShapingMode)
	{
		return NoiseShapingMode;
	}
	else if (index == kQuantize)
	{
		return Quantize;
	}
	else if (index == kQuantizationMode)
	{
		return QuantizationMode;
	}
	else if (index == kOnlyError)
	{
		return OnlyError;
	}
	else if (index == kAutoDither)
	{
		return AutoDither;
	}
	else if (index == kInvertDither)
	{
		return InvertDither;
	}
	else if (index == kSeed)
	{
		return Seed/1000;
	}
	else if (index == kSeedWithTime)
	{
		return SeedWithTime;
	}
	else if (index == kClipPreQuantization)
	{
		return ClipPreQuantization;
	}
	else if (index == kClipValue)
	{
		return ClipValue;
	}
	else if (index == kClipPostQuantization)
	{
		return ClipPostQuantization;
	}
	else if (index == kInGain)
	{
		return InGain;
	}
	else if (index == kOutGain)
	{
		return OutGain;
	}
	else if (index == kDitherGain)
	{
		return DitherGain;
	}
	else if (index == kNoiseShapingGain)
	{
		return NoiseShapingGain;
	}
	return 0;
}

void C1Bitcrusher::getParameterDisplay (VstInt32 index, char* text)
{
	if (index == kDisable)
	{
		if (Disable >= 0.5)	
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
	}
	else if (index == kBitDepth)
	{
		float2string (BitDepth, text, kVstMaxParamStrLen);
	}
	else if (index == kDCBias)
	{
		float2string (DCBias, text, kVstMaxParamStrLen);
	}
	else if (index == kDither)
	{
		if (Dither >= 0.5)	
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
	}
	else if (index == kDitherType)
	{
		if (DitherType >= 0.0 && DitherType < 0.25)	
		{
			strcpy (text, "Rectangular");
		}
		else if (DitherType >= 0.25 && DitherType < 0.5)	
		{
			strcpy (text, "Triangular");
		}
		else
		{
			strcpy (text, "Gaussian");
		}
	}
	else if (index == kDitherInError)
	{
		if (DitherInError >= 0.5)	
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
	}
	else if (index == kNoiseShaping)
	{
		if (NoiseShaping >= 0.5)	
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
	}
	else if (index == kNoiseShapingMode)
	{
		if (NoiseShapingMode >= 0.0 && NoiseShapingMode < 0.5)	
		{
			strcpy (text, "Low");
		}
		else
		{
			strcpy (text, "High");
		}
	}
	else if (index == kQuantize)
	{
		if (Quantize >= 0.5)	
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
	}
	else if (index == kQuantizationMode)
	{
		if (QuantizationMode >= 0.0 && QuantizationMode < 0.25)	
		{
			strcpy (text, "Floor");
		}
		else if (QuantizationMode >= 0.25 && QuantizationMode < 0.5)	
		{
			strcpy (text, "Ceiling");
		}
		else if (QuantizationMode >= 0.5 && QuantizationMode < 0.75)	
		{
			strcpy (text, "Truncate");
		}
		else
		{
			strcpy (text, "Round");
		}
	}
	else if (index == kOnlyError)
	{
		if (OnlyError >= 0.5)	
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
	}
	else if (index == kAutoDither)
	{
		if (AutoDither >= 0.5)	
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
	}
	else if (index == kInvertDither)
	{
		if (InvertDither >= 0.5)	
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
	}
	else if (index == kSeed)
	{
		int2string ((int)Seed, text, kVstMaxParamStrLen);
	}
	else if (index == kSeedWithTime)
	{
		if (SeedWithTime >= 0.5)	
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
	}
	else if (index == kClipPreQuantization)
	{
		if (ClipPreQuantization >= 0.5)	
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
	}
	else if (index == kClipValue)
	{
		dB2string (ClipValue, text, kVstMaxParamStrLen);
	}
	else if (index == kClipPostQuantization)
	{
		if (ClipPostQuantization >= 0.5)	
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
	}
	else if (index == kInGain)
	{
		float2string (InGain, text, kVstMaxParamStrLen);
	}
	else if (index == kOutGain)
	{
		float2string (OutGain, text, kVstMaxParamStrLen);
	}
	else if (index == kDitherGain)
	{
		float2string (DitherGain, text, kVstMaxParamStrLen);
	}
	else if (index == kNoiseShapingGain)
	{
		float2string (NoiseShapingGain, text, kVstMaxParamStrLen);
	}
}

void C1Bitcrusher::getParameterLabel (VstInt32 index, char* label)
{
	if (index == kBitDepth)
	{
		strcpy (label, "Bits");
	}
	else if (index == kDCBias)
	{
		strcpy (label, "LSB");
	}
	else if (index == kSeed)
	{
		strcpy (label, "I");
	}
	else if (index == kClipValue)
	{
		strcpy (label, "dB");
	}
	else if (index == kInGain)
	{
		strcpy (label, "F");
	}
	else if (index == kOutGain)
	{
		strcpy (label, "F");
	}
	else if (index == kDitherGain)
	{
		strcpy (label, "F");
	}
	else if (index == kNoiseShapingGain)
	{
		strcpy (label, "F");
	}
}

void C1Bitcrusher::getParameterName (VstInt32 index, char* text)
{
	if (index == kDisable)
	{
		strcpy (text, "Disable");
	}
	else if (index == kBitDepth)
	{
		strcpy (text, "BitDepth");
	}
	else if (index == kDCBias)
	{
		strcpy (text, "DCBias");
	}
	else if (index == kDither)
	{
		strcpy (text, "Dither");
	}
	else if (index == kDitherType)
	{
		strcpy (text, "DitherType");
	}
	else if (index == kDitherInError)
	{
		strcpy (text, "DitherInError");
	}
	else if (index == kNoiseShaping)
	{
		strcpy (text, "NoiseShaping");
	}
	else if (index == kNoiseShapingMode)
	{
		strcpy (text, "NoiseShapingMode");
	}
	else if (index == kQuantize)
	{
		strcpy (text, "Quantize");
	}
	else if (index == kQuantizationMode)
	{
		strcpy (text, "QuantizationMode");
	}
	else if (index == kOnlyError)
	{
		strcpy (text, "OnlyError");
	}
	else if (index == kAutoDither)
	{
		strcpy (text, "AutoDither");
	}
	else if (index == kInvertDither)
	{
		strcpy (text, "InvertDither");
	}
	else if (index == kSeed)
	{
		strcpy (text, "Seed");
	}
	else if (index == kSeedWithTime)
	{
		strcpy (text, "SeedWithTime");
	}
	else if (index == kClipPreQuantization)
	{
		strcpy (text, "ClipPreQuantization");
	}
	else if (index == kClipValue)
	{
		strcpy (text, "ClipValue");
	}
	else if (index == kClipPostQuantization)
	{
		strcpy (text, "ClipPostQuantization");
	}
	else if (index == kInGain)
	{
		strcpy (text, "InGain");
	}
	else if (index == kOutGain)
	{
		strcpy (text, "OutGain");
	}
	else if (index == kDitherGain)
	{
		strcpy (text, "DitherGain");
	}
	else if (index == kNoiseShapingGain)
	{
		strcpy (text, "NoiseShapingGain");
	}
}

bool C1Bitcrusher::getEffectName (char* name)
{
	strcpy (name, "C1Bitcrusher");
	return true;
}

bool C1Bitcrusher::getProductString (char* text)
{
	strcpy (text, "C1Bitcrusher");
	return true;
}

bool C1Bitcrusher::getVendorString (char* text)
{
	strcpy (text, "Datajake");
	return true;
}

float C1Bitcrusher::RPDF()
{
	return rand() / (float)(RAND_MAX/2);
}

float C1Bitcrusher::TPDF()
{
	float s1 = rand() / (float)(RAND_MAX/2);
	float s2 = rand() / (float)(RAND_MAX/2);
	return s1 + s2;
}

#define PI 3.1415926536

float C1Bitcrusher::AWGN_generator()
{/* Generates additive white Gaussian Noise samples with zero mean and a standard deviation of 1. */

	float temp1;
	float temp2;
	float result;
	int p;

	p = 1;

	while( p > 0 )
	{
		temp2 = ( rand() / ( (float)RAND_MAX ) ); /*  rand() function generates an
													integer between 0 and  RAND_MAX,
													which is defined in stdlib.h.
												*/

		if ( temp2 == 0 )
		{// temp2 is >= (RAND_MAX / 2)
			p = 1;
		}// end if
		else
		{// temp2 is < (RAND_MAX / 2)
			p = -1;
		}// end else

	}// end while()

	temp1 = cosf( ( 2.0f * (float)PI ) * rand() / ( (float)RAND_MAX ) );
	result = sqrtf( -2.0f * logf( temp2 ) ) * temp1;

	return result;	// return the generated random sample to the caller

}// end AWGN_generator()

float C1Bitcrusher::DitherNoise()
{
	if (DitherType >= 0.0 && DitherType < 0.25)
	{
		return RPDF();
	}
	else if (DitherType >= 0.25 && DitherType < 0.5)
	{
		return TPDF();
	}
	else
	{
		return AWGN_generator();
	}
}

float C1Bitcrusher::DitherSample(float sample)
{
	float noise;
	if (sample == 0 && AutoDither >= 0.5)
	{
		noise = 0;
	}
	else
	{
		noise = DitherNoise() / NumAmplitudes;
	}
	noise = noise * DitherGain;
	if (InvertDither >= 0.5)
	{
		noise = noise * -1;
	}
	return sample + noise;
}

float C1Bitcrusher::ClipSample(float sample)
{
	if (sample > ClipValue)
	{
		sample = ClipValue;
	}
	else if (sample < ClipValue * -1)
	{
		sample = ClipValue * -1;
	}
	return sample;
}

float C1Bitcrusher::QuantizeSample(float sample)
{
	float scale = NumAmplitudes/2;
	sample = sample * scale;
	if (QuantizationMode >= 0.0 && QuantizationMode < 0.25)
	{
		sample = floorf(sample);
	}
	else if (QuantizationMode >= 0.25 && QuantizationMode < 0.5)
	{
		sample = ceilf(sample);
	}
	else if (QuantizationMode >= 0.5 && QuantizationMode < 0.75)
	{
		if (sample >= 0)
		sample = floorf(sample);
		else
		sample = ceilf(sample);
	}
	else
	{
		if (sample >= 0)
		sample = floorf(sample + 0.5f);
		else
		sample = ceilf(sample - 0.5f);
	}
	if (ClipPostQuantization >= 0.5)
	{
		if (sample > scale - 1)
		sample = scale - 1;
		else if (sample < scale * -1)
		sample = scale * -1;
	}
	sample = sample / scale;
	return sample;
}

void C1Bitcrusher::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* in1 = inputs[0];
	float* in2 = inputs[1];
	float* out1 = outputs[0];
	float* out2 = outputs[1];
	if (Disable >= 0.5)
	{
		for (int i=0; i<sampleFrames; i++)
		{
			*out1 = *in1;
			*out2 = *in2;
			*in1++;
			*in2++;
			*out1++;
			*out2++;
		}
		return;
	}
	for (int i=0; i<sampleFrames; i++)
	{
		*out1 = *in1 * InGain;
		*out2 = *in2 * InGain;
		*out1 = *out1 + DCBias/NumAmplitudes;
		*out2 = *out2 + DCBias/NumAmplitudes;
		if (Dither >= 0.5 && DitherInError < 0.5)
		{
			*out1 = DitherSample(*out1);
			*out2 = DitherSample(*out2);
		}
		if (Quantize >= 0.5)
		{
			if (NoiseShaping >= 0.5)
			{
				if (NoiseShapingMode >= 0.5)
				{
					*out1 = *out1 - error[0] * NoiseShapingGain;
					*out2 = *out2 - error[1] * NoiseShapingGain;
				}
				else
				{
					*out1 = *out1 + error[0] * NoiseShapingGain;
					*out2 = *out2 + error[1] * NoiseShapingGain;
				}
			}
			if (ClipPreQuantization >= 0.5)
			{
				*out1 = ClipSample(*out1);
				*out2 = ClipSample(*out2);
			}
			if (Dither >= 0.5 && DitherInError >= 0.5)
			{
				quantized[0] = QuantizeSample(DitherSample(*out1));
				quantized[1] = QuantizeSample(DitherSample(*out2));
			}
			else
			{
				quantized[0] = QuantizeSample(*out1);
				quantized[1] = QuantizeSample(*out2);
			}
			error[0] = quantized[0] - *out1;
			error[1] = quantized[1] - *out2;
			if (OnlyError >= 0.5)
			{
				*out1 = error[0];
				*out2 = error[1];
			}
			else
			{
				*out1 = quantized[0];
				*out2 = quantized[1];
			}
		}
		*out1 = *out1 * OutGain;
		*out2 = *out2 * OutGain;
		*in1++;
		*in2++;
		*out1++;
		*out2++;
	}
}
