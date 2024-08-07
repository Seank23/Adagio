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
        // m_FileIOService.LoadAudio("C:\\Users\\seank\\Documents\\Audacity\\Guitar Freq Analysis.wav", FileFormat::WAV, wavAudio);

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
}
