using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.UI;

public class SelfDestruction1 : MonoBehaviour
{
	private InputAction playerInput;
	private GameObject playerObj;

	[SerializeField] private InputActionReference selfDestructionAction;

	// ゲージのUIとしてスプライト画像のリストを使用
	[SerializeField] private Image gaugeImage;
	[SerializeField] private List<Sprite> gaugeSprites; // 59枚のスプライトを格納するリスト

	// スプライトがあるフォルダパス
	private string spriteFolderPath = "UITexture/jibaku";

	void Start()
    {
		// 入力アクションの確認
		if (selfDestructionAction == null)
		{
			return;
		}

		// プレイヤーオブジェクトを初期化
		playerObj = GameObject.FindGameObjectWithTag("Player");
		playerInput = selfDestructionAction.action;
		playerInput.Enable();

		// gaugeImageがnullならUIオブジェクトを再取得する
		if (gaugeImage == null)
		{
			GameObject uiObj = GameObject.Find("JibakuUI");  // JibakuUI を探す
			if (uiObj != null)
			{
				gaugeImage = uiObj.GetComponent<Image>();    // Image コンポーネントを取得
			}
		}
		// スプライトのリストを読み込む
		LoadSprites();
	}

	// スプライトを読み込んでリストに格納する
	private void LoadSprites()
	{
		gaugeSprites = new List<Sprite>();

		for (int i = 0; i <= 59; i++)
		{
			// スプライト名は "jibaku_00000", "jibaku_00001", ... となっている
			string spriteName = string.Format("{0}/jibaku_{1:D5}", spriteFolderPath, i);
			Sprite sprite = Resources.Load<Sprite>(spriteName);
			if (sprite != null)
			{
				gaugeSprites.Add(sprite);
			}
			else
			{
				Debug.LogError("スプライトが読み込めません: " + spriteName);
			}
		}
	}

	// Update is called once per frame
	void Update()
    {
        if (playerInput == null) return;

		// リスポーン後のプレイヤーを再取得
		if (playerObj == null)
		{
			playerObj = GameObject.FindGameObjectWithTag("Player");
			if (playerObj == null)
			{
				return;
			}
		}

		if (playerInput.triggered)
		{
			// 自爆が始まる前にUIをリセット
			StartSelfDestructionSequence();
			ResetGaugeAnimation();
		}

		// プレイヤーの近くにゲージを表示
		Vector3 playerPos = playerObj.transform.position;
		Vector3 gaugePos = playerPos + new Vector3(0, 0, 0);
		gaugeImage.transform.position = Camera.main.WorldToScreenPoint(gaugePos);


		// 長押しの進捗を取得
		var holdProgress = playerInput.GetTimeoutCompletionPercentage();

		// ゲージのUIに進捗を反映する
		// スプライトを進捗に応じて変更する
		int spriteIndex = Mathf.FloorToInt(holdProgress * (gaugeSprites.Count - 1));
		if (spriteIndex >= 0 && spriteIndex < gaugeSprites.Count)
		{
			gaugeImage.sprite = gaugeSprites[spriteIndex];
		}

		// 自爆が完了したらUIを非表示にする
		//if (holdProgress >= 1.0f)
		//{
		//	gaugeImage.gameObject.SetActive(false); // 自爆完了時にUIを非アクティブにする
		//}
	}

	void StartSelfDestructionSequence()
	{
		// gaugeImageがnullならUIオブジェクトを再取得する
		// gaugeImage が null の場合は再取得
		if (gaugeImage == null)
		{
			GameObject uiObj = GameObject.Find("JibakuUI");
			if (uiObj != null)
			{
				gaugeImage = uiObj.GetComponent<Image>();    // Image コンポーネントを取得
			}
		}
		gaugeImage.gameObject.SetActive(true); // UIを再度アクティブにする
		gaugeImage.fillAmount = 0; // ゲージの初期状態にリセット
	}

	void ResetGaugeAnimation()
	{
		if (gaugeSprites != null && gaugeSprites.Count > 0)
		{
			// 最初のスプライトを設定する
			gaugeImage.sprite = gaugeSprites[0];
		}
	}
}
