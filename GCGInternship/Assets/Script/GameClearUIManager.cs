using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using UnityEngine.InputSystem;

public class GameClearUIManager : MonoBehaviour
{
    public RectTransform gameClearImage;     // ゲームクリアのメインImage
    public RectTransform quitImage;          // 終了ボタンImage
    public RectTransform BImage;
    public Image blackFade;                  // ESC:おわる用のフェードImage

    public AudioClip escSound;             // ESCキーの効果音
    public AudioClip gameClearSound;       // ゲームクリア効果音
    private AudioSource audioSource;       // 効果音再生用のAudioSource

    private bool isGameClear = false;

    void Start()
    {
        audioSource = gameObject.AddComponent<AudioSource>();
        blackFade.gameObject.SetActive(false);
    }

    void Update()
    {
        // キーボード取得
        var current = Keyboard.current;
        var escKey = current.escapeKey;

        // Xboxコントローラーが接続されているか確認
        var gamePad = Gamepad.current;
        bool buttonBPressed = gamePad != null && gamePad.bButton.wasPressedThisFrame;

        if (!isGameClear) return;

        if (buttonBPressed || escKey.wasPressedThisFrame)
        {
            PlaySound(escSound);
            StartCoroutine(QuitToTitle()); // blackFadeでフェード
        }
    }

    public void TriggerGameClear()
    {
        isGameClear = true;
        PlaySound(gameClearSound); // ゲームクリア時の音を再生
        StartCoroutine(SlideInUI());
    }

    // UIを順番にスライドインさせるコルーチン
    private IEnumerator SlideInUI()
    {
        yield return SlideIn(gameClearImage, 623, 100, 0.5f); // Game Over Image
        yield return new WaitForSeconds(2f);                  // 2秒待機
        yield return SlideIn(quitImage, -700, -98, 0.5f);
        yield return SlideIn(BImage, -719, -98, 0.5f);
    }

    // UI Imageをスライドインさせるための共通メソッド
    private IEnumerator SlideIn(RectTransform uiElement, float startY, float endY, float duration)
    {
        Vector2 startPosition = new Vector2(uiElement.anchoredPosition.x, startY);
        Vector2 endPosition = new Vector2(uiElement.anchoredPosition.x, endY);
        float elapsedTime = 0f;

        while (elapsedTime < duration)
        {
            uiElement.anchoredPosition = Vector2.Lerp(startPosition, endPosition, elapsedTime / duration);
            elapsedTime += Time.deltaTime;
            yield return null;
        }

        uiElement.anchoredPosition = endPosition;
    }

    private IEnumerator QuitToTitle()
    {
        yield return FadeOut(blackFade, 1.0f); // 黒フェードアウト
        SceneManager.LoadScene("Title"); // タイトルシーン
        yield return FadeIn(blackFade, 1.0f);  // 黒フェードイン
    }
    // フェードアウト（不透明→透明）
    IEnumerator FadeOut(Image fadeImage, float duration)
    {
        fadeImage.gameObject.SetActive(true);
        Color color = fadeImage.color;
        for (float t = 0; t < duration; t += Time.deltaTime)
        {
            color.a = Mathf.Lerp(0, 1, t / duration);
            fadeImage.color = color;
            yield return null;
        }
        color.a = 1;
        fadeImage.color = color;
    }

    // フェードイン（透明→不透明）
    IEnumerator FadeIn(Image fadeImage, float duration)
    {
        Color color = fadeImage.color;
        for (float t = 0; t < duration; t += Time.deltaTime)
        {
            color.a = Mathf.Lerp(1, 0, t / duration);
            fadeImage.color = color;
            yield return null;
        }
        color.a = 0;
        fadeImage.color = color;
        fadeImage.gameObject.SetActive(false);
    }

    //指定SE再生処理
    private void PlaySound(AudioClip clip)
    {
        if (audioSource != null && clip != null)
        {
            audioSource.PlayOneShot(clip);
        }
    }
}