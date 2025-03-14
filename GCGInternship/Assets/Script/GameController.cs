using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using System.Collections;

public class GameController : MonoBehaviour
{
    public GameObject paperPlane;
    public GameObject goalTextUI;
    public GameObject endTextUI;
    public GameObject fadePanel;
    public Transform groundPosition;
    private bool isGoalReached = false;
    private bool isGameEnded = false;
    private bool isFading = false;

    void Start()
    {
        goalTextUI.SetActive(false);
        endTextUI.SetActive(false);
        fadePanel.SetActive(false);
    }

    void Update()
    {
        // エンターキーを押したときにゲームオーバーをトリガー
        if (Input.GetKeyDown(KeyCode.S) && !isFading)
        {
            StartCoroutine(ShowGoalTextUI());
            isGameEnded = true;
            ShowEndTextUI();  // ESC:おわるの表示
        }

        if (isGoalReached && !isGameEnded && paperPlane.transform.position.y <= groundPosition.position.y)
        {
            isGameEnded = true;
            ShowEndTextUI();  // ESC:おわるの表示
        }

        if (isGameEnded && Input.GetKeyDown(KeyCode.Escape) && !isFading)
        {
            StartCoroutine(EndGameSequence());
        }
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject == paperPlane)
        {
            isGoalReached = true;
            Rigidbody2D rb = paperPlane.GetComponent<Rigidbody2D>();
            rb.gravityScale = 1;
            StartCoroutine(ShowGoalTextUI());
        }
    }

    IEnumerator ShowGoalTextUI()
    {
        goalTextUI.SetActive(true);
        RectTransform rect = goalTextUI.GetComponent<RectTransform>();

        float time = 0;
        Vector3 startPos = new Vector3(0, Screen.height + 50, 0);
        Vector3 endPos = new Vector3(0, Screen.height * 0.3f, 0);

        while (time < 1)
        {
            time += Time.deltaTime;
            rect.localPosition = Vector3.Lerp(startPos, endPos, time);
            yield return null;
        }
    }

    void ShowEndTextUI()
    {
        endTextUI.SetActive(true);
        RectTransform endTextRect = endTextUI.GetComponent<RectTransform>();
        endTextRect.anchoredPosition = new Vector2(0, -Screen.height * 0.4f);
    }

    IEnumerator EndGameSequence()
    {
        isFading = true;
        fadePanel.SetActive(true);
        Image fadeImage = fadePanel.GetComponent<Image>();

        float fadeTime = 1.0f;
        for (float t = 0; t < fadeTime; t += Time.deltaTime)
        {
            fadeImage.color = new Color(0, 0, 0, t / fadeTime);
            yield return null;
        }

        SceneManager.LoadScene("Title");
    }
}
