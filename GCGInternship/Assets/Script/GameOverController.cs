using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using System.Collections;
using UnityEngine.InputSystem;

public class GameOverController : MonoBehaviour
{
    public RectTransform gameOverImage;      // ゲームオーバーのメインImage
    public RectTransform retryImage;         // リトライボタンImage
    public RectTransform quitImage;          // 終了ボタンImage
    public RectTransform AImage;
    public RectTransform BImage;
    public Image blackFade;                  // ESC:おわる用のフェードImage
    public Image whiteFade;                  // Enter:リトライ用のフェードImage

    public AudioClip enterSound;           // Enterキーの効果音
    public AudioClip escSound;             // ESCキーの効果音
    public AudioClip gameOverSound;       // ゲームオーバー効果音
    private AudioSource audioSource;       // 効果音再生用のAudioSource

    private bool isGameOver = false;

    void Start()
    {
        audioSource = gameObject.AddComponent<AudioSource>();
        blackFade.gameObject.SetActive(false);
        whiteFade.gameObject.SetActive(false);
    }

    void Update()
    {
        // キーボード取得
        var current = Keyboard.current;
        var returnKey = current.enterKey;
        var escKey = current.escapeKey;

        // Xboxコントローラーが接続されているか確認
        var gamePad = Gamepad.current;
        bool buttonAPressed = gamePad != null && gamePad.aButton.wasPressedThisFrame;
        bool buttonBPressed = gamePad != null && gamePad.bButton.wasPressedThisFrame;

        if (!isGameOver) return;
        if (buttonAPressed || returnKey.wasPressedThisFrame) // Enterキーでリトライ
        {
            PlaySound(enterSound);
            StartCoroutine(RestartGame());
        }

        else if (buttonBPressed || escKey.wasPressedThisFrame) // ESCキーで終了
        {
            PlaySound(escSound);
            StartCoroutine(QuitToTitle()); // blackFadeでフェード
        }
    }

    public void TriggerGameOver()
    {
        isGameOver = true;
        PlaySound(gameOverSound); // ゲームオーバー時の音を再生
        StartCoroutine(SlideInUI());
    }

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

    private IEnumerator SlideInUI()
    {
        yield return SlideIn(gameOverImage, 578, 100, 0.5f); // Game Over Image
        yield return new WaitForSeconds(2f);                  // 2秒待機
        yield return SlideIn(retryImage, -580, -98, 0.5f);    // リトライ Image
        yield return SlideIn(AImage, -599, -98, 0.5f);
        yield return SlideIn(quitImage, -700, -210, 0.5f);    // 終了 Image
        yield return SlideIn(BImage, -719, -210, 0.5f);
    }

    private IEnumerator RestartGame()
    {
        yield return FadeOut(whiteFade, 1.0f); // 白フェードアウト
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex); // リトライ
        yield return FadeIn(whiteFade, 1.0f);  // 白フェードイン
    }

    private IEnumerator QuitToTitle()
    {
        yield return FadeOut(blackFade, 1.0f); // 黒フェードアウト
        SceneManager.LoadScene("Title"); // タイトルシーン名に置き換える
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
