using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;  // TextMeshPro用の名前空間を追加
using UnityEngine.SceneManagement;

public class ResultTimer : MonoBehaviour
{
	public TextMeshProUGUI resultText;

	private string timerScene = "ResultScene"; // タイマーを表示、停止するシーン名

	private void Start()
	{
		// シーン名を取得
		Scene scene = SceneManager.GetActiveScene();
		string sceneName = scene.name;

		// シーン名がタイマーを表示、停止するシーン名と一致した場合
		if (sceneName == timerScene)
		{
			// タイマーを表示
			GameManager.Instance.StopTimer();
		}

	}

	//private void Update()
	//{
	//	// タイマーを表示、停止するシーン名と一致した場合
	//	if (SceneManager.GetActiveScene().name == timerScene)
	//	{
	//		// タイマーを停止
	//		GameManager.Instance.StopTimer();
	//	}

	//}


	void Draw()
	{
		// 累積時間を取得
		float totalElapsedTime = GameManager.Instance.GetTotalElapsedTime();

		// 分、秒に変換して表示
		float minutes = Mathf.FloorToInt(totalElapsedTime / 60);
		float seconds = Mathf.FloorToInt(totalElapsedTime % 60);
		resultText.text = string.Format("Total Time: {0:00}:{1:00}", minutes, seconds);
	}
}
