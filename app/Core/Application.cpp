#include "app/adagio_pch.h"
#include "Application.h"

#include "../IO/AudioData.h"
#include "AudioUtils.h"

namespace Adagio
{
	Application::Application()
	{
		ADAGIO_PROFILE_BEGIN_SESSION("Application", "Application_Profile.json");
        // AudioData wavAudio;
        // m_FileIOService.LoadAudio("C:\\Users\\seank\\Documents\\Audacity\\Guitar Freq Analysis.wav", FileFormat::WAV, m_AudioData);

		/*AudioData mp3Audio;
		m_FileIOService.LoadAudio("C:\\Users\\seank\\Documents\\Music\\Music Analysis\\Jazz\\Dont Fence Me In - Bing Crosby  The Andrews Sisters.mp3", FileFormat::MP3, mp3Audio);*/

        //AnalysisParams analysisParams;
        //analysisParams.SampleRate = 8000.0f;
        //m_AnalysisService.Init(wavAudio, analysisParams);

        // m_PlaybackService.InitAudio(wavAudio);
        // m_PlaybackService.PlayAudio();

        //uint64_t sampleCount = m_PlaybackService.GetSampleCount();
        //uint64_t currentSample = 0;
        // while (currentSample < sampleCount)
        // {
        // 	currentSample = m_PlaybackService.GetCurrentSample();
        // 	auto result = m_AnalysisService.AnalyseFrame(currentSample * (analysisParams.SampleRate / wavAudio.PlaybackSampleRate));
        // 	std::cout << m_PlaybackService.GetCurrentSample() << " - " << *std::max_element(result.begin(), result.end()) << '\n';
        // }
	}

	Application::~Application()
	{
        ADAGIO_PROFILE_END_SESSION();
	}

    int Application::LoadAudio(std::string filePath)
    {
        try
        {
            m_AudioData = std::make_shared<AudioData>();
            std::filesystem::path path = filePath;
            auto extension = path.extension();
            if (extension == ".wav")
                m_FileIOService.LoadAudio(path.string(), FileFormat::WAV, *m_AudioData);
            else if (extension == ".mp3")
                m_FileIOService.LoadAudio(path.string(), FileFormat::MP3, *m_AudioData);
            else if (extension == ".flac")
                m_FileIOService.LoadAudio(path.string(), FileFormat::FLAC, *m_AudioData);
        } catch (...)
        {
            return -1;
        }
        if (m_AudioData != nullptr)
            m_AudioLoaded = true;
        return 1;
    }

    int Application::ClearAudio()
    {
        try
        {
            m_AudioData = nullptr;
            m_AudioLoaded = false;
        } catch (...)
        {
            return -1;
        }
        return 1;
    }
}
