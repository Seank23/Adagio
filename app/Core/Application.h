#pragma once

#include "../IO/PlaybackService.h"
#include "../IO/FileIOService.h"
#include "../Analysis/AnalysisService.h"

namespace Adagio
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

        int LoadAudio(std::string filePath);
        int ClearAudio();

	private:
        FileIOService m_FileIOService;
		PlaybackService m_PlaybackService;
		AnalysisService m_AnalysisService;

        std::shared_ptr<AudioData> m_AudioData;
        bool m_AudioLoaded = false;
	};
}
