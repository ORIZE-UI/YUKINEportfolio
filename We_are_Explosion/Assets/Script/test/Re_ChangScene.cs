using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
#if UNITY_EDITOR
using UnityEngine.InputSystem;
#endif

public class Re_ChangScene : MonoBehaviour
{
	public static Re_ChangScene Instance { get; private set; } // シングルトンのインスタンス

	public string next = "Stage2_V2_1"; // 次のシーンの名前
	public string NowScene = "Stage";   // 現在のシーンの一時的な保存先
	int No = 2;                         // ステージカウント

	private DestructibleObject[] destructibleObjects; // 破壊可能オブジェクトの配列

	private bool isDestroyed = false; // オブジェクトが破壊されたかどうか

	// Start is called before the first frame update
	void Start()
	{

		destructibleObjects = new DestructibleObject[transform.childCount];
		foreach (Transform child in transform)
		{
			DestructibleObject destructibleObject = child.GetComponent<DestructibleObject>();
			if (destructibleObject != null)
			{
				destructibleObjects[child.GetSiblingIndex()] = destructibleObject;
			}
		}

		//NowScene = SceneManager.GetActiveScene().name;

		isDestroyed = false; // オブジェクトが破壊されたかどうかを初期化

	}
	// Update is called once per frame
	void Update()
	{

		// オブジェクトが１つでも破壊されたら
		foreach (DestructibleObject destructibleObject in destructibleObjects)
		{
			// 1回でも破壊されたオブジェクトがあれば次のシーンに遷移
			if (destructibleObject.IsDestroyed)
			{
				if (isDestroyed == false)
				{
					LoadSceneAsync(next);

					isDestroyed = true;
				}
			}
		}

	}

	// ロードシーンからnextを参照しゲームシーンに移行
	public void LoadStageScene()
	{
		SceneManager.LoadScene(next);
	}

	// タイトルに戻る
	public void LoadTitleScene()
	{
		SceneManager.LoadScene("TitleScene");
	}

	// ゲームクリアの方
	public void LoadResultScene()
	{
		SceneManager.LoadScene("ResultScene");
	}

	// ゲームオーバーに飛ぶ関数
	public void LoadGameOverScene()
	{
		SceneManager.LoadScene("GameOverScene");
	}

	//各ステージにロードシーン移行関数を適用
	public void LoadLoadingScene()
	{
		SetnextScene();
		SceneManager.LoadScene("LoadingScene");
	}

	// 次のシーンを用意する
	public void SetnextScene()
	{
		Scene currentScene = SceneManager.GetActiveScene();
		NowScene = currentScene.name;
		next = NowScene;
		// nextの末尾1文字を削除
		string modifiedNext = next.Substring(0, next.Length - 1);   // 一文字消す⇒ナンバー
																	// int型の変数numを文字列に変換して末尾に追加
		modifiedNext += No.ToString();
		next = modifiedNext;
		No += 1;
	}

	// ロード画面を表示し、非同期でシーンを読み込む
	public void LoadSceneAsync(string sceneName)
	{
		// ロード画面を表示
		SceneManager.LoadScene("LoadingScene");
		Scene load = SceneManager.GetSceneByName("LoadingScene");
		//SceneManager.SetActiveScene(load);


		// 非同期でシーンを読み込む
		StartCoroutine(LoadNextSceneAsync(sceneName));

	}

	// 非同期でシーンを読み込む + 一定時間経過後にシーン遷移
	IEnumerator LoadNextSceneAsync(string sceneName)
	{
		AsyncOperation operation = SceneManager.LoadSceneAsync(sceneName);
		operation.allowSceneActivation = false; // シーン遷移を待つ

		float startTime = Time.time; // シーン遷移開始時間

		while (!operation.isDone)
		{
			// 読み込み

			var lateTime = Time.time - startTime;
			
			if (operation.progress >= 0.9f && lateTime >= 2.5f)
			{
				// シーン遷移
				operation.allowSceneActivation = true;
			}

			yield return null; // 1フレーム待つ
		}
	}
}

