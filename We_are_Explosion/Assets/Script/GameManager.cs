using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.SceneManagement;
using UnityEngine.UI; // 追加：UI要素を使用するために必要
using TMPro;  // TextMeshProを使うために追加

public class GameManager : MonoBehaviour
{
	public static GameManager Instance { get; private set; }

	[Tooltip("プレイヤーのプレハブを設定します")]
	public GameObject playerPrefab; // プレイヤーのプレハブ

	//----2024/6/26 追加 近藤志音----
	// TimerManagerの機能を統合
	public TextMeshProUGUI timerText;
	private float elapsedTime = 0f;			// 現在のステージの経過時間
	private float totalElapsedTime = 0f;    // 全てのステージの累積時間
	private bool isRunning = false;

	[Tooltip("残機数を設定します")]
	public int initialLives = 3;        // 初期残機数
	private int currentLives;           // 現在の残機数

	[Tooltip("残機UIの画像を設定します")]
	public Image livesImage;            // 残機を表示するUIの画像

	[Tooltip("残機に応じたスプライトを設定します")]
	public Sprite blueSprite;           // 残機が3のときのスプライト
	public Sprite yellowSprite;         // 残機が2のときのスプライト
	public Sprite redSprite;            // 残機が1のときのスプライト


	private void Awake()
	{
		if (Instance == null)
		{
			Instance = this;
			DontDestroyOnLoad(gameObject); // シーンをまたいでオブジェクトを保持
			SceneManager.sceneLoaded += OnSceneLoaded; // シーンロード時に呼び出されるイベントを登録

			// TimerManagerの役割を追加
			elapsedTime = 0f;
			isRunning = false;
		}
		else
		{
			Destroy(gameObject);
		}
		if (timerText == null) // タイマーテキストを再取得する
		{
			timerText = GameObject.Find("Timer").GetComponent<TextMeshProUGUI>();
		}
		UpdateTimerText();
		ResetLives(); // 初期残機数を設定
		if (livesImage == null) // UIオブジェクトを再取得する
		{
			livesImage = GameObject.Find("RemainLife").GetComponent<Image>();
		}
		UpdateLivesUI(); // UIを更新
	}

	private void OnSceneLoaded(Scene scene, LoadSceneMode mode)
	{
		StartTimer();
		if (timerText == null) // タイマーテキストを再取得する
		{
			timerText = GameObject.Find("Timer").GetComponent<TextMeshProUGUI>();
		}
		UpdateTimerText();
		ResetLives(); // 新しいシーンがロードされたときに残機数をリセット
		if (livesImage == null) // UIオブジェクトを再取得する
		{
			livesImage = GameObject.Find("RemainLife").GetComponent<Image>();
		}
		UpdateLivesUI(); // UIを更新
	}

	public void ResetLives()
	{
		currentLives = initialLives;
	}

	public void StartRespawnCoroutine(float delay, Vector3 spawnPosition)
	{
		StartCoroutine(RespawnPlayerAfterDelay(delay, spawnPosition));
	}

	private IEnumerator RespawnPlayerAfterDelay(float delay, Vector3 spawnPosition)
	{
		Debug.Log("RespawnPlayerAfterDelay開始。" + delay + "秒待機します。");
		yield return new WaitForSeconds(delay);
		Debug.Log("待機完了。プレイヤーをリスポーンします。");

		if (playerPrefab != null)
		{
			if (currentLives > 0)
			{
				GameObject newPlayer = Instantiate(playerPrefab, spawnPosition, Quaternion.identity);
				newPlayer.name = playerPrefab.name; // オブジェクトの名前を元のプレハブの名前に設定
				SetupPlayer(newPlayer);
				Debug.Log("プレイヤーがリスポーンしました。");

				// 自爆範囲を表すUIを再アクティブにする
				GameObject uiObj = GameObject.Find("JibakuUI");
				if (uiObj != null)
				{
					uiObj.SetActive(true); // リスポーン後に UI をアクティブに戻す
				}

				currentLives--; // 残機数を減少
				UpdateLivesUI(); // UIを更新
				if (currentLives <= 0)
				{
					GameOver();
				}
			}
			else
			{
				GameOver();
			}
		}
		else
		{
			Debug.LogError("プレイヤープレハブがゲームマネージャーに割り当てられていません！");
		}
	}

	private void SetupPlayer(GameObject player)
	{
		EnableScript<SelfDestruction>(player);
		EnableScript<PlayerMove>(player);
		EnableScript<PlayerDeathOnBulletCollision>(player);
		EnableScript<PlayerInput>(player);
	}

	private void EnableScript<T>(GameObject player) where T : MonoBehaviour
	{
		T script = player.GetComponent<T>();
		if (script != null)
		{
			script.enabled = true;
			Debug.Log($"{typeof(T).Name} スクリプトが有効化されました。");
		}
		else
		{
			Debug.LogWarning($"{typeof(T).Name} スクリプトが新しいプレイヤーに見つかりません。");
		}
	}
	//-----2024/6/26 追加 近藤志音-----
	//--------タイマー処理----------
	void Update()
	{
		if (isRunning)
		{
			elapsedTime += Time.deltaTime;  // 経過時間を加算
			UpdateTimerText();
		}
	}
	// タイマーを開始
	public void StartTimer()
	{
		isRunning = true;
	}
	// タイマーを停止
	public void StopTimer()
	{
		isRunning = false;
		totalElapsedTime += elapsedTime;  // 累積時間に加算
	}
	// タイマーのリセット
	public void ResetTimer()
	{
		elapsedTime = 0f;
		totalElapsedTime = 0f;
		UpdateTimerText();
	}
	// タイマーのUIを更新
	private void UpdateTimerText()
	{
		if (timerText != null)
		{
			float minutes = Mathf.FloorToInt(elapsedTime / 60);
			float seconds = Mathf.FloorToInt(elapsedTime % 60);
			timerText.text = string.Format("{0:00}:{1:00}", minutes, seconds);
		}
	}
	public float GetTotalElapsedTime()
	{
		return totalElapsedTime;  // 累積時間を返す
	}
	//------------------------------------------
	private void UpdateLivesUI()
	{
		if (livesImage != null)
		{
			switch (currentLives)
			{
				case 3:
					livesImage.sprite = blueSprite;
					break;
				case 2:
					livesImage.sprite = yellowSprite;
					break;
				case 1:
					livesImage.sprite = redSprite;
					break;
				case 0:
					livesImage.enabled = false; // 残機が0になったらUIを非表示にする場合
					break;
				default:
					Debug.LogError("不正な残機数です: " + currentLives);
					break;
			}
		}
		else
		{
			Debug.LogError("livesImageが割り当てられていません！");
		}
	}

	private void GameOver()
	{
		Debug.Log("ゲームオーバーです。");
		Awake();
		SceneManager.LoadScene("GameOverScene"); // "GameOverScene"というシーンに遷移
		StopTimer();
		ResetTimer();		 // タイマーをリセット
	}

}
