#pragma once

#include <kfr/base.hpp>

namespace Adagio
{
	class AudioData
	{
	public:
		AudioData() {}
		AudioData(kfr::univector2d<float> playbackStream)
			: PlaybackStream(playbackStream)
		{

		}

		kfr::univector2d<float> PlaybackStream;
		kfr::univector<float> AnalysisStream;

		float PlaybackSampleRate = 0.0f;
		float AnalysisSampleRate = 0.0f;

		int Channels = 0;
		float Duration = 0.0f;
	};
}
