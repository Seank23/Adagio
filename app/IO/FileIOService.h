#pragma once

#include "../Core/AudioUtils.h"
#include "AudioFileHandler.h"

namespace Adagio
{
	class FileIOService
	{
	public:
		FileIOService();
		void LoadAudio(std::string filepath, FileFormat format, AudioData& o_Audio);

	private:
		AudioFileHandler m_AudioFileHandler;
	};
}

