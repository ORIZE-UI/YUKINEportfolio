using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ChangeScene : MonoBehaviour
{
    public float displayTime = 2.0f; // 挿絵シーンの表示時間
    void Update()
    {
		if (ReloadScene.Instance != null)
		{
			if (GameManager.Instance != null)
			{
				GameManager.Instance.ResetLives();
			}

			// ReloadScene スクリプトのインスタンスを取得して次のシーンをロード
			ReloadScene.Instance.LoadNextScene();
		}
		else
		{
			Debug.LogError("ReloadScene スクリプトが見つかりません");
		}
    }
}

