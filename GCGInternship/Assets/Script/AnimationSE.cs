using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AnimationSE : MonoBehaviour
{
    [SerializeField]
    private AudioSource audioSource;  // AudioSourceをInspectorで設定
    [SerializeField]
    private AudioClip[] AnimeSounds;  // 複数のAudioClipを設定できるようにする

    // アニメーションイベントで呼ばれる関数（パラメータでSEを指定）
    public void PlayAnimeSound(int soundIndex)
    {
        // soundIndexが範囲内か確認
        if (audioSource != null && AnimeSounds != null && soundIndex >= 0 && soundIndex < AnimeSounds.Length)
        {
            audioSource.PlayOneShot(AnimeSounds[soundIndex]); // 指定されたSEを再生
        }
    }
}