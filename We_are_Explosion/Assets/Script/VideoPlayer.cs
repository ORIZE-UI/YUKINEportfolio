using System;
using System.Collections;
using UnityEngine;
using UnityEngine.Video;

public class VideoPlayerController : MonoBehaviour
{
	public VideoPlayer videoPlayer;  // VideoPlayerコンポーネントをアタッチ
	public GameObject gameoverObject; // Gameoverオブジェクト

	void Start()
	{
		// VideoPlayerのループをオフに設定
		videoPlayer.isLooping = false;

		// 動画が終了したときに呼び出されるイベントを設定
		videoPlayer.loopPointReached += OnVideoFinished;

		// Gameoverオブジェクトを非表示にする
		gameoverObject.SetActive(false);

		// 動画を再生
		videoPlayer.Play();
	}

	void OnVideoFinished(VideoPlayer vp)
	{
		// 動画を非表示にする
		videoPlayer.gameObject.SetActive(false);

		// Gameoverオブジェクトを表示する
		gameoverObject.SetActive(true);
	}
}
