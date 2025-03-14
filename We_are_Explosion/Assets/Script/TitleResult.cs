using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class TitleResult : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

	// Update is called once per frame
	void Update()
	{
		// ステージへ遷移する(エンターキー)
		if (Input.GetKeyDown(KeyCode.Return))
		{
			SceneManager.LoadScene("Stage2_1_V2");
		}
		// ステージへ遷移する(XboxコントローラーのAボタン)
		if (Input.GetKeyDown(KeyCode.JoystickButton0))
		{
			SceneManager.LoadScene("Stage2_1_V2");
		}

		// タイトルへ戻る(バックスペース)
		if (Input.GetKeyDown(KeyCode.Backspace))
		{
			SceneManager.LoadScene("TitleScene");
		}
	}

	//void OnCollisionEnter(Collision collision)
	//{
	//	SceneManager.LoadScene("GameClearScene");
	//}
}
