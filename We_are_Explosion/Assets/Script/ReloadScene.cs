using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ReloadScene : MonoBehaviour
{
	public static ReloadScene Instance { get; private set; }

	private int currentSceneIndex = 0; // 現在のシーンインデックス
	private bool isResultScreen = false;	// リザルトシーンからの遷移を追跡するフラグ

	public List<string> sceneNames = new List<string>
	{ "Stage2_1_V2", "Stage2_2_V2", "Stage2_3_V2",
		"Stage2_4_V2", "Stage2_5_V2","Stage2_4" }; // 遷移先シーン名のリスト

	public string loadingSceneName = "LoadingScene";    // 挿絵シーン名

	//public List<Vector3> startPositions = new List<Vector3>
	//{
	//	new Vector3(-7.34f, 7.51f, -0.5f),
	//	new Vector3(-9.32f, -3.66f, -0.5f),
	//	new Vector3(11.87f, -3.95f, -0.5f),
	//	new Vector3(0.0f, 0.0f, 0.0f),
	//	new Vector3(0.0f, 0.1f, 0.0f),
	//	new Vector3(0.0f, 0.0f, 0.1f),
	//};

	private DestructibleObject[] destructibleObjects; // 破壊可能オブジェクトの配列

	private bool isDestroyed = false; // オブジェクトが破壊されたかどうか

	private void Awake()
	{
		// シングルトンパターンの実装
		if (Instance == null)
		{
			Instance = this;
			DontDestroyOnLoad(gameObject); // シーンがロードされてもオブジェクトを破棄しない
		}
		else
		{
			//Destroy(gameObject); // 既にインスタンスが存在する場合は新しいものを破棄
		}

		destructibleObjects = new DestructibleObject[transform.childCount];
		foreach (Transform child in transform)
		{
			DestructibleObject destructibleObject = child.GetComponent<DestructibleObject>();
			if (destructibleObject != null)
			{
				destructibleObjects[child.GetSiblingIndex()] = destructibleObject;
			}
		}

		isDestroyed = false; // オブジェクトが破壊されたかどうかを初期化
	}

	// Start is called before the first frame update
	void Start()
	{
		if (sceneNames == null || sceneNames.Count == 0)
		{
			Debug.LogError("シーン名リストが設定されていません");
		}
	}

	// Update is called once per frame
	void Update()
	{
		if (isDestroyed == false)
		{
			// オブジェクトが１つでも破壊されたら
			foreach (DestructibleObject destructibleObject in destructibleObjects)
			{
				// 1回でも破壊されたオブジェクトがあれば次のシーンに遷移
				if (destructibleObject.IsDestroyed)
				{
					FadeManager fadeManager = FindObjectOfType<FadeManager>();

					if (isDestroyed == false)
					{
						if (fadeManager != null)
						{
							fadeManager.FadeToScene(loadingSceneName);
						}
						else
						{
							SceneManager.LoadScene(loadingSceneName);
						}
						isDestroyed = true;
					}
				}
			}
		}
	}

	// 次のシーンをロードするメソッド
	public void LoadNextScene()
	{
		if (isResultScreen)
		{
			currentSceneIndex = 0; // リザルト画面から戻る時は最初のシーンにリセット
			isResultScreen = false; // フラグをリセット
		}
		else
		{
			currentSceneIndex = (currentSceneIndex + 1) % sceneNames.Count; // 次のシーンに遷移
		}

		string nextScene = sceneNames[currentSceneIndex];
		SceneManager.LoadScene(nextScene); // 次のシーンをロード
		StartCoroutine(ResetObjectPosition(nextScene)); // オブジェクトの位置をリセット
	}

	//public void LoadNextScene()
	//{
	//	if (sceneNames != null && sceneNames.Count > 0)
	//	{
	//		// 現在のシーンインデックスを更新して次のシーンを選択
	//		currentSceneIndex = (currentSceneIndex + 1) % sceneNames.Count;
	//		string nextScene = sceneNames[currentSceneIndex];

	//		// 残機をリセット
	//		if (GameManager.Instance != null)
	//		{
	//			GameManager.Instance.ResetLives();
	//		}

	//		SceneManager.LoadScene(nextScene);
	//		// シーンロード後にオブジェクトの位置をリセット
	//		StartCoroutine(ResetObjectPosition(nextScene));

	//	}
	//	else
	//	{
	//		Debug.LogError("シーン名リストが空です");
	//	}
	//}

	// リザルト画面から戻る際にフラグを設定するメソッド
	public void SetResultScreenFlag(bool isResult)
	{
		isResultScreen = isResult; // リザルト画面からの遷移フラグを設定
	}

	private IEnumerator ResetObjectPosition(string sceneName)
	{
		// シーンのロード完了を待つ
		while (!SceneManager.GetActiveScene().name.Equals(sceneName))
		{
			yield return null;
		}

		//// オブジェクトの位置を初期位置にリセット
		//if (initialPositions.ContainsKey(sceneName))
		//{
		//	transform.position = initialPositions[sceneName];
		//}
	}
}
