using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.SceneManagement;

public class SteageReset : MonoBehaviour
{
	private PlayerInput playerInput;

	// Start is called before the first frame update
	void Start()
    {
		playerInput = GetComponent<PlayerInput>();
	}

	// Update is called once per frame
	void Update()
    {
		var SteageReset = playerInput.actions["SteageReset"].WasPerformedThisFrame();
		if (SteageReset)
		{
			ResetCurrentScene();
		}
	}

	// シーンをリセットするメソッド
	public void ResetCurrentScene()
	{
		// 現在のシーンを再読み込み
		Scene currentScene = SceneManager.GetActiveScene();
		SceneManager.LoadScene(currentScene.name);
	}
}
