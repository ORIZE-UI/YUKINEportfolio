using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.InputSystem;
//using Re_ChangeScene

public class Re_ChangeSceneObject : MonoBehaviour
{
	public string sceneName;    // シーンの名前

	private Re_ChangScene changeScene;

	// Start is called before the first frame update
	void Start()
    {
		// 現在のシーン名を取得して格納
		Scene currentScene = SceneManager.GetActiveScene();
		sceneName = currentScene.name;

		changeScene = new Re_ChangScene();
	}

	// Update is called once per frame
	void Update()
    {
		// ======ゲームタイトル時の遷移先======
		if (sceneName == "TitleScene")
		{
			// キーボード取得
			var current = Keyboard.current;
			var returnKey = current.enterKey;
			// コントローラーボタン取得
			if (Gamepad.current != null)
			{
				var gamePad = Gamepad.current;
				var buttonA = gamePad.buttonSouth;

				if (buttonA.wasPressedThisFrame)
				{
					changeScene.LoadStageScene();  // ゲームシーンの関数呼び出し
				}
			}

			// エンターキー又はXBoxコンのAボタンを押してゲームシーンへ遷移する
			if (current.enterKey.wasPressedThisFrame)
			{
				changeScene.LoadStageScene();  // ゲームシーンの関数呼び出し
			}
		}

		// =======ゲームクリア時の遷移先======
		if (sceneName == "ResultScene")
		{
			// キーボード取得
			var current = Keyboard.current;
			var returnKey = current.enterKey;
			// コントローラーボタン取得
            var gamePad = Gamepad.current;
            var buttonA = gamePad.buttonSouth;

			// エンターキー又はXBoxコンのAボタンを押してタイトルシーンへ遷移する
			if (current.enterKey.wasPressedThisFrame || buttonA.wasPressedThisFrame)
			{
				changeScene.LoadTitleScene();   // タイトルシーンの関数呼び出し
			}
		}

		// ======ゲームオーバー時の遷移先======
		if (sceneName == "GameOverScene")
		{
			// キーボード取得
			var current = Keyboard.current;
			var returnKey = current.enterKey;
			var backspaceKey = current.backspaceKey;
			// コントローラーボタン取得
            var gamePad = Gamepad.current;
            var buttonA = gamePad.buttonSouth;
            var buttonSelect = gamePad.selectButton;

            // エンターキー又はXBoxコンのAボタンを押してゲームシーンに戻る
            if (current.enterKey.wasPressedThisFrame || buttonA.wasPressedThisFrame)
			{
				changeScene.LoadStageScene();	// ゲームシーンの関数呼び出し
			}
            // バックスペースXBoxコンのSelectボタンを押してタイトルに戻る
            else if (current.backspaceKey.wasPressedThisFrame || buttonSelect.wasPressedThisFrame)
			{
				changeScene.LoadTitleScene();  // タイトルシーンの関数呼び出し
			}
		}
    }
}
