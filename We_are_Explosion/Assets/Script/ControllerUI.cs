using System.Collections;
using UnityEngine;
using UnityEngine.InputSystem;

public class ControllerUI : MonoBehaviour
{
	[SerializeField] private RectTransform uiPanel; // 対象のUIのRectTransform
	[SerializeField] private float slideDuration = 0.5f; // アニメーションの所要時間
	[SerializeField] private Vector2 hiddenPosition = new Vector2(0, 1068); // UIが隠れているときの位置
	[SerializeField] private Vector2 visiblePosition = new Vector2(0, -71.5f); // UIが表示されているときの位置

	private bool isVisible = false; // UIが表示されているかのフラグ
	
	// UpdateメソッドでInputActionsのClickを監視する
	void Update()
	{
		if (Mouse.current.leftButton.wasPressedThisFrame)		
		{
			ToggleUI(); // UIを表示/非表示に切り替える
		}
		if(Gamepad.current != null && Gamepad.current.startButton.wasPressedThisFrame)
		{
			ToggleUI(); // UIを表示/非表示に切り替える
		}
	}

	// UIの表示/非表示をトグルで切り替える
	private void ToggleUI()
	{
		StopAllCoroutines(); // 他のアニメーションが実行中の場合、停止する
		if (isVisible)
		{
			StartCoroutine(SlideUI(hiddenPosition)); // 隠れる位置に移動
		}
		else
		{
			StartCoroutine(SlideUI(visiblePosition)); // 表示位置に移動
		}
		isVisible = !isVisible; // 表示状態を反転
	}

	// UIを滑らかに移動させるコルーチン
	private IEnumerator SlideUI(Vector2 targetPosition)
	{
		Vector2 startPosition = uiPanel.anchoredPosition;
		float elapsedTime = 0;

		while (elapsedTime < slideDuration)
		{
			// 線形補間で滑らかに移動させる
			uiPanel.anchoredPosition = Vector2.Lerp(startPosition, targetPosition, elapsedTime / slideDuration);
			elapsedTime += Time.deltaTime;
			yield return null;
		}

		// 最終的に正確な位置に配置
		uiPanel.anchoredPosition = targetPosition;
	}
}
