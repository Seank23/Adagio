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

	private:
		FileIOService m_FileIOService;
		PlaybackService m_PlaybackService;
		AnalysisService m_AnalysisService;
	};
}
