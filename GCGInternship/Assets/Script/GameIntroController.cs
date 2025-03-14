using System.Collections;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Video;
using UnityEngine.SceneManagement; // シーン遷移に必要
using TMPro; // TextMeshProの名前空間
using UnityEngine.InputSystem;

public class GameIntroController : MonoBehaviour
{
    public GameObject titleScreen;     // タイトル画像
    public VideoPlayer videoPlayer;    // ムービー再生用
    public GameObject slideImage;      // スライドインする一枚絵 
    public GameObject rulesUI;         // ゲームルールUI
    public Image startButton;         // ボタンコンポーネント
    public TextMeshProUGUI enterText;  // 「ENTER」のTextMeshPro UI
    public Image blackFade;            // 黒フェード用Image 
    public Image whiteFade;            // 白フェード用Image 
    public string gameSceneName;       // 遷移先のゲームシーン名

    public AudioClip enterSound;       // Enterキーの効果音
    public AudioClip bgmTitle;         // タイトル画面のBGM
    public AudioClip bgmMovie;         // ムービー中のBGM
    private AudioSource bgmSource;     // BGM再生用AudioSource
    private AudioSource sfxSource;     // 効果音再生用AudioSource

    private bool isEnterPressed = false;

    void Start()
    {
        // AudioSourceコンポーネントを追加して設定
        bgmSource = gameObject.AddComponent<AudioSource>();
        sfxSource = gameObject.AddComponent<AudioSource>();
        bgmSource.loop = true;  // BGMはループ再生する
        sfxSource.playOnAwake = false;
        // タイトルBGMのフェードイン再生
        PlayBGM(bgmTitle, fadeInDuration: 1.0f);

        // 初期設定
        startButton.gameObject.SetActive(true);
        blackFade.gameObject.SetActive(false);
        whiteFade.gameObject.SetActive(false);
        enterText.gameObject.SetActive(true);
        slideImage.SetActive(false);
        rulesUI.SetActive(false); // ゲームルールUIを非表示
        StartCoroutine(FadeOut(blackFade, 1.0f)); // 初回フェードイン

        // VideoPlayer終了後のコールバック設定
        videoPlayer.loopPointReached += OnVideoEnd;

        // VideoPlayer再生開始時のコールバック設定
        videoPlayer.started += OnVideoStarted;
    }

    void Update()
    {
        // キーボード取得
        var current = Keyboard.current;
        var returnKey = current.enterKey;

        // Xboxコントローラーが接続されているか確認
        var gamePad = Gamepad.current;
        bool buttonAPressed = gamePad != null && gamePad.aButton.wasPressedThisFrame;

        // タイトルでEnterが押されたときの処理
        if (!isEnterPressed &&
            (buttonAPressed || returnKey.wasPressedThisFrame))
        {
            PlaySound(enterSound);
            isEnterPressed = true;
            StartCoroutine(TransitionToVideo());
            startButton.gameObject.SetActive(false);
            enterText.gameObject.SetActive(false);
            rulesUI.SetActive(false); // ゲームルールUIを非表示
        }

        // ゲームルールUI表示後に何らかの入力があった場合、ゲームシーンへ遷移
        if (rulesUI.activeInHierarchy &&
            (buttonAPressed || returnKey.wasPressedThisFrame))
        {
            StartCoroutine(TransitionToGameScene());
        }
    }



    IEnumerator TransitionToVideo()
    {
        // タイトル画面からムービーへの遷移
        yield return FadeIn(blackFade, 1.0f); // 黒フェードアウト
        titleScreen.SetActive(false);

        // タイトルBGMをフェードアウトし、ムービーBGMを再生
        StartCoroutine(FadeOutBGM(1.0f));

        videoPlayer.gameObject.SetActive(true);
        yield return FadeOut(blackFade, 1.0f); // 黒フェードイン
        videoPlayer.Play();
    }

    // 動画再生開始と同時にBGM_movieをフェードイン再生
    void OnVideoStarted(VideoPlayer vp)
    {
        PlayBGM(bgmMovie, fadeInDuration: 1.0f); // ムービーBGMのフェードイン再生
    }

    void OnVideoEnd(VideoPlayer vp)
    {
        // ムービー終了後に一枚絵をスライドイン
        StartCoroutine(SlideInImage());
    }

    IEnumerator SlideInImage()
    {
        // ムービー終了後、黒フェードアウトで一枚絵を右からスライドイン
        yield return FadeIn(blackFade, 1.0f); // 黒フェードアウト

        videoPlayer.gameObject.SetActive(false);
        slideImage.SetActive(true);
        Vector3 startPosition = new Vector3(Screen.width, 0, 0);
        slideImage.transform.localPosition = startPosition;

        float slideDuration = 1.0f;
        Vector3 endPosition = Vector3.zero;
        float elapsed = 0f;

        while (elapsed < slideDuration)
        {
            slideImage.transform.localPosition = Vector3.Lerp(startPosition, endPosition, elapsed / slideDuration);
            elapsed += Time.deltaTime;
            yield return null;
        }
        slideImage.transform.localPosition = endPosition;

        yield return FadeOut(blackFade, 1.0f); // 黒フェードイン
        // 2秒後にゲームルールUIを右からスライドイン
        yield return new WaitForSeconds(2.0f);
        StartCoroutine(SlideInRulesUI());
    }
    IEnumerator SlideInRulesUI()
    {
        // ゲームルールUIを右からスライドイン
        rulesUI.SetActive(true);
        Vector3 startPosition = new Vector3(Screen.width, 0, 0);
        rulesUI.transform.localPosition = startPosition;

        float slideDuration = 1.0f;
        Vector3 endPosition = Vector3.zero;
        float elapsed = 0f;

        while (elapsed < slideDuration)
        {
            rulesUI.transform.localPosition = Vector3.Lerp(startPosition, endPosition, elapsed / slideDuration);
            elapsed += Time.deltaTime;
            yield return null;
        }
        rulesUI.transform.localPosition = endPosition;
    }

    IEnumerator TransitionToGameScene()
    {
        // 白フェードアウト後、シーン遷移
        yield return FadeIn(whiteFade, 1.0f); // 白フェードアウト
        SceneManager.LoadScene(gameSceneName); // シーン遷移
    }

    // BGMをフェードイン再生するメソッド
    private void PlayBGM(AudioClip clip, float fadeInDuration)
    {
        bgmSource.clip = clip;
        bgmSource.volume = 0;
        bgmSource.Play();
        StartCoroutine(FadeInBGM(fadeInDuration));
    }

    // フェードインでBGM音量を上げる
    private IEnumerator FadeInBGM(float duration)
    {
        float elapsedTime = 0f;
        float startVolume = 0.6f;
        while (elapsedTime < duration)
        {
            bgmSource.volume = Mathf.Lerp(0, startVolume, elapsedTime / duration);
            elapsedTime += Time.deltaTime;
            yield return null;
        }
        bgmSource.volume = startVolume;
    }

    // フェードアウトでBGM音量を下げる
    private IEnumerator FadeOutBGM(float duration)
    {
        float startVolume = bgmSource.volume;
        float elapsedTime = 0f;
        while (elapsedTime < duration)
        {
            bgmSource.volume = Mathf.Lerp(startVolume, 0, elapsedTime / duration);
            elapsedTime += Time.deltaTime;
            yield return null;
        }
        bgmSource.Stop();
        bgmSource.volume = 0.6f; // フェードアウト後に音量をリセット
    }

    // フェードアウト（透明→不透明）
    IEnumerator FadeOut(Image fadeImage, float duration)
    {
        fadeImage.gameObject.SetActive(true); // フェード処理中のみ表示
        Color color = fadeImage.color;
        for (float t = 0; t < duration; t += Time.deltaTime)
        {
            color.a = Mathf.Lerp(1, 0, t / duration);
            fadeImage.color = color;
            yield return null;
        }
        color.a = 0;
        fadeImage.color = color;
        fadeImage.gameObject.SetActive(false); // フェード完了後に非表示
    }

    // フェードイン（不透明→透明）
    IEnumerator FadeIn(Image fadeImage, float duration)
    {
        fadeImage.gameObject.SetActive(true); // フェード処理中のみ表示
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

    // 指定SE再生処理
    private void PlaySound(AudioClip clip)
    {
        if (sfxSource != null && clip != null)
        {
            sfxSource.PlayOneShot(clip);
        }
    }
}
