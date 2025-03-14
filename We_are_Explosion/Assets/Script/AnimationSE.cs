using UnityEngine;

public class AnimationSE : MonoBehaviour
{
	[SerializeField]
	private AudioSource audioSource;  // AudioSourceをInspectorで設定
	[SerializeField]
	private AudioClip[] footstepSounds;  // 複数のAudioClipを設定できるようにする

	// アニメーションイベントで呼ばれる関数（パラメータでSEを指定）
	public void PlayFootstepSound(int soundIndex)
	{
		// soundIndexが範囲内か確認（安全策）
		if (audioSource != null && footstepSounds != null && soundIndex >= 0 && soundIndex < footstepSounds.Length)
		{
			audioSource.PlayOneShot(footstepSounds[soundIndex]); // 指定されたSEを再生
		}
		else
		{
			Debug.LogWarning("AudioSourceまたはfootstepSoundsが設定されていない、もしくはsoundIndexが不正です。");
		}
	}
}
