using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShutterControll : MonoBehaviour
{
	[Tooltip("シャッターのオブジェクトを入れてください.")]
	public Transform Shutter; // シャッターのTransform
	[Tooltip("シャッターの上昇値を設定してください.")]
	public float ShutterUp = 4.0f; //シャッターの上昇値
	public List<string> switchTags = new () { "Switch" };
	public float duration = 1.0f; // アニメーションの時間
	public int paturn = 1;  //アニメーションのパターン
	private Vector3 originalPos;  // 初期座標
	private Vector3 originalScale; // 初期スケール
	private bool isOpen = false; // シャッターが開いているかどうかを示すフラグ

	void Start()
	{
		originalPos = Shutter.localPosition; // 初期座標を保存
		originalScale = Shutter.localScale; // 初期スケールを保存
	}
	
	void OnCollisionStay(Collision other)
	{
		foreach (var switchTag in switchTags)  // プレイヤーがスイッチのオブジェクトに接触したら
		{
			if (isOpen == false)
			{
				isOpen = true;
				if (paturn == 1)
				{
					StartCoroutine(AnimateTransUp());
				}
				else if(paturn == 2)
				{
					StartCoroutine(AnimateScaleDown());
				}
			}
		}
	}
	void OnCollisionExit(Collision other)
	{
		foreach (var switchTag in switchTags) // プレイヤーが当たり判定から離れた場合
		{
			isOpen = false;
			if (Shutter.localPosition.y > originalPos.y && isOpen == false)
			{
				if (paturn == 1)
				{
					StartCoroutine(AnimateTransDown());
				}
				else if (paturn == 2)
				{
					StartCoroutine(AnimateScaleUp());
				}
			}
		}
	}
	IEnumerator AnimateTransUp()
	{
		while (Shutter.localPosition.y < originalPos.y + ShutterUp)
		{
			Shutter.Translate(Vector3.up * 0.5f * Time.deltaTime, Space.World); // 毎フレームごとに上昇させる
			yield return null;
		}
	}

	IEnumerator AnimateTransDown()
	{
		while (Shutter.localPosition.y > originalPos.y)
		{
			Shutter.Translate(Vector3.down * 0.5f * Time.deltaTime, Space.World); // 毎フレームごとに下降させる
			yield return null;
		}
	}

	IEnumerator AnimateScaleDown()
	{
		float timer = 0f;
		while (timer < duration)
		{
			timer += Time.deltaTime;
			// 時間経過に応じてシャッターのYスケールを変更する
			float newYScale = Mathf.Lerp(originalScale.y, 0f, timer / duration);
			Shutter.localScale = new Vector3(originalScale.x, newYScale, originalScale.z);
			Shutter.Translate(Vector3.up * 0.4f * Time.deltaTime, Space.World); // 毎フレームごとに上昇させる

			yield return null;
		}
	}

	IEnumerator AnimateScaleUp()
	{
		float timer = 0f;

		while(timer < duration)
		//while (Shutter.localScale.y <= originalScale.y && Shutter.localPosition.y < originalPos.y)
			{
			timer += Time.deltaTime;
			// 時間経過に応じてシャッターのYスケールを変更する
			float newYScale = Mathf.Lerp(0f, originalScale.y, timer / duration);
			Shutter.localScale = new Vector3(originalScale.x, newYScale, originalScale.z);

			// 同時にY座標を下げる
			Shutter.Translate(Vector3.down * 0.25f * Time.deltaTime, Space.World); // 毎フレームごとに下降させる

			yield return null;
		}
	}
}
