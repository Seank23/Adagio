#pragma once

#include "../IO/PlaybackService.h"
#include "../IO/FileIOService.h"
#include "../Analysis/AnalysisService.h"
#include "../API/Utils.h"

namespace Adagio
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

        int LoadAudio(std::string filePath);
        int ClearAudio();

        void UpdateAudioState(PlayState state);
        PlayState GetAudioState() { return m_AudioState; }

        std::vector<float> ConstructWaveformData();
        float GetPlaybackSampleRate() { return m_AudioData->PlaybackSampleRate; }

	private:
        FileIOService m_FileIOService;
		PlaybackService m_PlaybackService;
		AnalysisService m_AnalysisService;

        AudioData* m_AudioData;
        bool m_AudioLoaded = false;

        PlayState m_AudioState;
	};
}
