using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using System.Collections;

public class FadeManager : MonoBehaviour
{
	public Image fadeImage;
	public float fadeDuration = 1f;

	private void Start()
	{
		if (fadeImage != null)
		{
			fadeImage.color = new Color(0, 0, 0, 1);
			StartCoroutine(FadeIn());
		}
	}

	public void FadeToScene(string sceneName)
	{
		StartCoroutine(FadeOutAndLoadScene(sceneName));
	}

	private IEnumerator FadeIn()
	{
		float elapsedTime = 0f;
		while (elapsedTime < fadeDuration)
		{
			fadeImage.color = new Color(0, 0, 0, 1 - (elapsedTime / fadeDuration));
			elapsedTime += Time.deltaTime;
			yield return null;
		}
		fadeImage.color = new Color(0, 0, 0, 0);
	}

	private IEnumerator FadeOutAndLoadScene(string sceneName)
	{
		float elapsedTime = 0f;
		while (elapsedTime < fadeDuration)
		{
			fadeImage.color = new Color(0, 0, 0, elapsedTime / fadeDuration);
			elapsedTime += Time.deltaTime;
			yield return null;
		}
		fadeImage.color = new Color(0, 0, 0, 1);
		SceneManager.LoadScene(sceneName);
	}
}

