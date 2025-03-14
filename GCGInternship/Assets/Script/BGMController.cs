using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BGMController : MonoBehaviour
{
    public AudioSource bgmAudioSource;       // BGM用のAudioSource
    public float fadeInDuration = 2.0f;      // フェードインの所要時間
    public float fadeOutDuration = 2.0f;     // フェードアウトの所要時間
    [HideInInspector] public float baseVolume = 1.0f; // 高度変化に使う基底音量

    void Start()
    {
        if (bgmAudioSource != null)
        {
            bgmAudioSource.volume = 0;
            baseVolume = 1.0f;  // 最大音量
            bgmAudioSource.Play();
            StartCoroutine(FadeIn());
        }
    }

    // フェードイン処理
    private IEnumerator FadeIn()
    {
        float elapsedTime = 0f;
        while (elapsedTime < fadeInDuration)
        {
            baseVolume = Mathf.Lerp(0, 1, elapsedTime / fadeInDuration); // 基底音量をフェードイン
            bgmAudioSource.volume = baseVolume;
            elapsedTime += Time.deltaTime;
            yield return null;
        }
        baseVolume = 1;
    }

    // フェードアウト処理
    public void StartFadeOut()
    {
        StartCoroutine(FadeOut());
    }

    private IEnumerator FadeOut()
    {
        float startVolume = baseVolume;
        float elapsedTime = 0f;
        while (elapsedTime < fadeOutDuration)
        {
            baseVolume = Mathf.Lerp(startVolume, 0, elapsedTime / fadeOutDuration); // 基底音量をフェードアウト
            bgmAudioSource.volume = baseVolume;
            elapsedTime += Time.deltaTime;
            yield return null;
        }
        baseVolume = 0;
        bgmAudioSource.Stop();
    }
}
