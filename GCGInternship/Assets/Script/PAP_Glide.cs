using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class PAP_Glide : MonoBehaviour
{
    //基本挙動用
    public float initialSpeed    = 16.0f;       // 初速
    public float dragCoefficient = -0.001f;     // 空気抵抗の係数
    public float liftCoefficient = 0.5f;        // 揚力の係数（少し減少）
    public float slideSpeed = 5.0f;             // スライドする速度
    public float tiltAngle  = 15.0f;            // 傾く角度の大きさ
    public float tiltSpeed  = 2.0f;             // 傾きの変化速度（スムーズにするため）
    public string groundTag = "TestField";      // 床オブジェクトのタグ
    public string draftTag  = "VerticalDraft";  // 上昇気流のタグ
    public string enemyTag  = "Enemy";          // 敵オブジェクトのタグ
    public string houseTag = "House";          // 敵オブジェクトのタグ
    public float groundFriction  = 0.98f;       // 地面との摩擦（減速率）
    public float minSlideSpeed   = 0.1f;        // 最低速度（この速度以下で停止）
    public float slideDecaySpeed = 0.95f;       // スライドの減速率

    //着地後用
    private bool hasLanded      = false; // 着地フラグ
    private bool isSliding      = false; // 滑っているかどうか
    private bool isAscending    = false; // 上昇中かどうかを判定するフラグ
    public float ascendingSpeed = 3.0f;  // 上昇時のY軸の速度
    private float currentTilt   = 0f;    // 現在の傾き角度
    public BGMController bgmController;  // BGMControllerを参照
    private GameOverController GOUI;       // GameOverUIManagerスクリプトの参照

    //各UI用
    public RectTransform meterUI;          // メーターUIのRectTransform
    public float meterMinY        = -370f; // メーターの最小位置（UIのY軸）
    public float meterMaxY        = 175f;  // メーターの最大位置（UIのY軸）
    public float planeMinAltitude = 0f;    // 紙飛行機の最低高度
    public float planeMaxAltitude = 130f;  // 紙飛行機の最高高度

    //物理演算用
    private Rigidbody rb;
    private float originalDrag;            // 元の空気抵抗を保存しておく
    private float originalLiftCoefficient; // 元の揚力係数を保存しておく

    //ゴール後用
    public string goalTag                 = "Goal";// ゴールオブジェクトのタグ
    public float smoothLandingSpeed       = 0.5f;  // ゴール時の滑らかに降下するスピード
    public float positionXTransitionSpeed = 1.0f;  // X座標を滑らかに0にする速度
    private bool isGoalLanding            = false; // ゴール時の着地フラグ
    private bool goalReached              = false; // ゴール到達のフラグ
    private bool isControlEnabled         = true;  // 操作を受け付けるかどうかのフラグ
    private Cam_PAPfollow cameraScript;            // カメラ追従スクリプトの参照

    // 上昇気流と敵生成の停止用
    public VerticalDraft draftScript; // 上昇気流生成のスクリプト参照
    public Proto_Vertical PdraScript; // 上昇気流生成のスクリプト参照
    public EnemySpawner enemySpawner; // 敵生成のスクリプト参照
    public BigEnemySpawner BenemySp;  // 大きな敵の生成スクリプト参照

    //吹き飛ば処理用
    public float throwForce         = 50.0f;          // 吹き飛ばされる力
    public float throwRotationSpeed = 180.0f;         // 回転速度
    public float throwDuration      = 1.0f;           // 吹き飛ばしの持続時間を1秒に設定
    public string bigBirdFrontTag   = "BigBirdFront"; // BigBirdの前の空オブジェクトのタグ
    private bool isThrown           = false;          // 吹き飛ばされた状態かどうか

    //各エフェクト用
    public GameObject rippleStarPrefab;   // Ripple_VFXエフェクトのプレハブ
    public GameObject HitImpactPrefab;    // HitImpact_VFXエフェクトのプレハブ

    //飛行軌跡エフェクト用
    public GameObject contrailPrefab;         // Contrails_VFXエフェクトのプレハブ
    private GameObject leftContrailInstance;   // 左翼用のエフェクトインスタンス
    private GameObject rightContrailInstance;  // 右翼用のエフェクトインスタンス
    
    // 翼の先端部分を表す手動指定の座標オフセット
    public Vector3 leftWingOffset = new Vector3(-0.5f, 0, -0.5f);  // 紙飛行機中心から左翼先端の位置
    public Vector3 rightWingOffset = new Vector3(0.5f, 0, -0.5f);   // 紙飛行機中心から右翼先端の位置

    // サウンドエフェクト用
    [SerializeField] private AudioSource audioSource;
    [SerializeField] private AudioClip windAscendSound;     // 上昇気流の音
    [SerializeField] private AudioClip birdCollisionSound;  // 鳥衝突の音
    [SerializeField] private AudioClip gustSound;           // 突風の音
    [SerializeField] private AudioClip controlReturnSound;  // 操作復帰の音
    [SerializeField] private AudioClip landingSound;        // 着地の音

    //滑空音用のAudioClip
    [SerializeField] private AudioClip glideClip; // 滑空音用のAudioClip
    private AudioSource glideSource;              // 内部的に使用するAudioSource
    private bool isGlidePlaying = false;          // 滑空音が再生中かどうかのフラグ
    private float glideFadeDuration = 5.0f;         // フェードの時間（秒）

    // 新たな変数を追加
    public float maxAscendingSpeed = 5.0f; // 上昇の最大速度（調整可能）
    public float constantSpeed = 16.0f;     // 一定速度で飛行するための固定速度
    private bool isInDraft = false;         // 上昇気流内にいるかどうかのフラグ
    void Start()
    {
        rb = GetComponent<Rigidbody>();

        // 初速を前方向に与える
        rb.velocity = transform.forward * constantSpeed;

        // 回転抵抗を調整して安定性を向上
        rb.angularDrag = 0.1f;

        // 元の空気抵抗と揚力を保存
        originalDrag = dragCoefficient;
        originalLiftCoefficient = liftCoefficient;

        // カメラのスクリプトを取得
        cameraScript = Camera.main.GetComponent<Cam_PAPfollow>();

        // 上昇気流と敵のスクリプトを取得
        bgmController = FindObjectOfType<BGMController>();
        draftScript = FindObjectOfType<VerticalDraft>();
        PdraScript = FindObjectOfType<Proto_Vertical>();
        enemySpawner = FindObjectOfType<EnemySpawner>();
        BenemySp = FindObjectOfType<BigEnemySpawner>();
        GOUI = FindObjectOfType<GameOverController>();

        // AudioSourceの設定
        glideSource = gameObject.AddComponent<AudioSource>();
        glideSource.clip = glideClip;
        glideSource.loop = true;
        glideSource.volume = 0;  // 初期音量は0

        StartCoroutine(FadeInGlideSound());  // ゲーム開始時にフェードイン

        // contrailエフェクトを紙飛行機の指定位置に生成
        if (contrailPrefab != null)
        {
            leftContrailInstance = Instantiate(contrailPrefab, transform.position + leftWingOffset, Quaternion.identity);
            rightContrailInstance = Instantiate(contrailPrefab, transform.position + rightWingOffset, Quaternion.identity);
        
            // 初期状態で非表示に
            leftContrailInstance.SetActive(false);
            rightContrailInstance.SetActive(false);
        }
    }

    void FixedUpdate()
    {
        if (!isControlEnabled || isThrown)
            return;

        if (!isGoalLanding)
        {
            // 上昇速度制限の追加
            if (rb.velocity.y > maxAscendingSpeed)
            {
                rb.velocity = new Vector3(rb.velocity.x, maxAscendingSpeed, rb.velocity.z);
            }

            HandleNormalFlight();
        }
    }

    private void HandleNormalFlight()
    {
        // 着地して滑っていない場合は飛行の処理
        if (hasLanded && !isSliding) return;

        if (!hasLanded)
        {
            // 飛行中はエフェクトを表示、停止時は非表示
            if (!leftContrailInstance.activeSelf) leftContrailInstance.SetActive(true);
            if (!rightContrailInstance.activeSelf) rightContrailInstance.SetActive(true);
            // 翼の先端位置を紙飛行機に追従させる
            leftContrailInstance.transform.position = transform.position + transform.rotation * leftWingOffset;
            rightContrailInstance.transform.position = transform.position + transform.rotation * rightWingOffset;


            // 紙飛行機の速度を固定化
            MaintainConstantSpeed();

            // 空気抵抗と揚力の調整
            ApplyAerodynamicForces();

            // 紙飛行機の現在の高度（Y軸の位置）を取得
            float currentAltitude = transform.position.y;

            // メーターUIを飛行高度に応じて上下に移動
            MoveMeterBasedOnAltitude(currentAltitude);

            // 揚力を追加
            if (!isAscending)  // 上昇気流に乗っていないときのみ揚力を調整
            {
                Vector3 lift = Vector3.up * rb.velocity.magnitude * liftCoefficient;
                rb.AddForce(lift);
            }

            // 空気抵抗を追加
            Vector3 drag = -rb.velocity.normalized * dragCoefficient * rb.velocity.sqrMagnitude;
            rb.AddForce(drag);

            // 重力の影響を調整
            //rb.AddForce(Physics.gravity);  // 重力の影響を強化

            // 左右のスライド操作と傾きを処理
            float tiltDirection = 0f;  // 傾きの方向を記録する変数

            // キーボード取得
            var current = Keyboard.current;
            var aKey = current.aKey;
            var dKey = current.dKey;

            // Xboxコントローラーが接続されているか確認
            var gamePad = Gamepad.current;
            bool Rightbutton = gamePad != null && gamePad.dpad.right.isPressed;
            bool Leftbutton = gamePad != null && gamePad.dpad.left.isPressed;
            // 左スティックの入力を取得
            float leftStickHorizontal = gamePad != null ? gamePad.leftStick.x.ReadValue() : 0f;

            // 左右のスライド操作を処理
            if (Leftbutton || aKey.isPressed || leftStickHorizontal < -0.5f)
            {
                // 左にスライド
                rb.AddForce(Vector3.left * slideSpeed, ForceMode.Force);
                tiltDirection = 1f;  // 左に傾く
            }
            else if (Rightbutton || dKey.isPressed || leftStickHorizontal > 0.5f)
            {
                // 右にスライド
                rb.AddForce(Vector3.right * slideSpeed, ForceMode.Force);
                tiltDirection = -1f;  // 右に傾く
            }
            

            // 現在の傾き角度を滑らかに変化させる（Z軸の回転）
            currentTilt = Mathf.Lerp(currentTilt, tiltDirection * tiltAngle, Time.fixedDeltaTime * tiltSpeed);

            // 紙飛行機の傾きをZ軸（Roll）に適用
            transform.localRotation =
                Quaternion.Euler(transform.localEulerAngles.x, transform.localEulerAngles.y, currentTilt);

            // 上昇中であればプレイヤーのY軸を上昇させる
            if (isAscending)
            {
                rb.velocity = new Vector3(rb.velocity.x, ascendingSpeed, constantSpeed);
            }
        }
        else if (isSliding)
        {
            if (leftContrailInstance.activeSelf) leftContrailInstance.SetActive(false);
            if (rightContrailInstance.activeSelf) rightContrailInstance.SetActive(false);

            // 滑り中の場合、速度を減衰させる
            rb.velocity = new Vector3(rb.velocity.x * slideDecaySpeed, rb.velocity.y, rb.velocity.z * slideDecaySpeed);

            // ある程度の速度まで減速したら滑りを停止
            if (rb.velocity.magnitude < minSlideSpeed)
            {
                rb.velocity = Vector3.zero;
                rb.angularVelocity = Vector3.zero;
                rb.isKinematic = true;  // 完全に停止
                isSliding = false;
            }
        }
    }

    private void MaintainConstantSpeed()
    {
        // 現在の速度をリセットして、Z軸方向の速度を一定にする
        rb.velocity = new Vector3(rb.velocity.x, rb.velocity.y, constantSpeed);
    }

    private void ApplyAerodynamicForces()
    {
        if (!isAscending)
        {
            Vector3 lift = Vector3.up * rb.velocity.magnitude * liftCoefficient;
            rb.AddForce(lift);
        }

        Vector3 drag = -rb.velocity.normalized * dragCoefficient * rb.velocity.sqrMagnitude;
        rb.AddForce(drag);

        rb.AddForce(Physics.gravity); // 重力の影響
    }

    void OnCollisionEnter(Collision collision)
    {
        // 敵と衝突した場合、エフェクトを再生
        if (collision.gameObject.CompareTag(enemyTag) || collision.gameObject.CompareTag(houseTag))
        {
            PlayHitEffect(collision.contacts[0].point); // 衝突地点でエフェクト再生
        }

        // 床に接触したかどうかを判定
        if (collision.gameObject.CompareTag(groundTag))
        {
            PlaySound(landingSound); // 着地音
            StartCoroutine(FadeOutGlideSound());  // 着地時にフェードアウト
            hasLanded = true;
            isSliding = true;  // スライド開始

            // ゴール以外で着陸した場合のみGameOverを呼び出す
            if (!goalReached && !isGoalLanding)
            {
                GameOver();  // ゲームオーバー処理を実行
            }

            // 着地時の速度を保持して、滑りの開始
            if (!isGoalLanding)
            {
                rb.velocity = rb.velocity * groundFriction;
            }
            else
            {
                rb.velocity = rb.velocity * 0;
            }

            // カメラに俯瞰視点への移行を指示
            if (cameraScript != null)
            {
                cameraScript.TransitionToGoalPosition();  // カメラの追従解除と移動開始
            }

            // 着地した瞬間にRipple_VFXを再生
            PlayRippleEffect();
        }
    }

    // RippleStarエフェクトを再生するメソッド
    void PlayRippleEffect()
    {
        if (rippleStarPrefab != null)
        {
            // エフェクトを紙飛行機の位置に生成
            GameObject rippleEffectInstance = Instantiate(rippleStarPrefab, transform.position, Quaternion.identity);
    
            // 子オブジェクトに含まれるすべてのParticleSystemを取得して再生
            ParticleSystem[] particleSystems = rippleEffectInstance.GetComponentsInChildren<ParticleSystem>();
            foreach (ParticleSystem ps in particleSystems)
            {
                ps.Play();
            }
    
            // VisualEffectコンポーネントが含まれる場合も再生
            var visualEffects = rippleEffectInstance.GetComponentsInChildren<UnityEngine.VFX.VisualEffect>();
            foreach (var vfx in visualEffects)
            {
                vfx.Play();
            }
    
            // エフェクトが終わった後にオブジェクトを削除
            Destroy(rippleEffectInstance, 5f); // 5秒後に削除
        }
    }

    // 衝突エフェクトを生成するメソッド
    private void PlayHitEffect(Vector3 position)
    {
        if (HitImpactPrefab != null)
        {
            Instantiate(HitImpactPrefab, position, Quaternion.identity);
        }
    }

    void OnTriggerEnter(Collider other)
    {
        // 上昇気流に接触したかどうかを判定
        if (other.CompareTag(draftTag))// && !isWithinDraft)
        {
            PlaySound(windAscendSound); // 上昇気流の音
            StartAscending();
            isInDraft = true;
            isAscending = true;
            rb.velocity = new Vector3(rb.velocity.x, ascendingSpeed, constantSpeed);
            //isWithinDraft = true; // 上昇中にする
        }

        if (other.CompareTag(enemyTag) || other.CompareTag(houseTag))
        {
            PlaySound(birdCollisionSound);  // 鳥衝突の音
            StartCoroutine(FadeOutGlideSound());  // 衝突時にフェードアウト
        }

        //吹き飛ばし判定
        if (other.CompareTag(bigBirdFrontTag))
        {
            Vector3 relativePosition = other.transform.InverseTransformPoint(transform.position);
            PlaySound(gustSound); // 突風の音
            StartCoroutine(FadeOutGlideSound());  // 突風に吹き飛ばされる際にフェードアウト
            if (relativePosition.x > 0)
            {
                // 右側に飛ばされる
                StartCoroutine(ThrowPlane(Vector3.right, throwForce));
            }
            else
            {
                // 左側に飛ばされる
                StartCoroutine(ThrowPlane(Vector3.left, throwForce));
            }
        }

        // ゴールの判定処理
        if (other.CompareTag(goalTag))
        {
            // ゴールに到達したとき
            OnGoalReached();
        }
    }

    void OnTriggerExit(Collider other)
    {
        if (other.CompareTag(draftTag))
        {
            StopAscending();
            isInDraft = false;
            isAscending = false;
            rb.velocity = new Vector3(rb.velocity.x, 0, constantSpeed);
        }
    }

    // 高度に基づいてメーターを上下に移動させる処理
    void MoveMeterBasedOnAltitude(float altitude)
    {
        if (meterUI != null)
        {
            // 紙飛行機の高度をメーターの位置にマッピングする
            float normalizedAltitude = Mathf.InverseLerp(planeMinAltitude, planeMaxAltitude, altitude);

            // メーターのUIのY軸位置を更新
            float newYPosition = Mathf.Lerp(meterMinY, meterMaxY, normalizedAltitude);

            // UIのY軸位置を更新
            meterUI.anchoredPosition = new Vector2(meterUI.anchoredPosition.x, newYPosition);
        }
    }

    // ゴールに到達した際に呼び出される処理
    public void OnGoalReached()
    {
        goalReached = true;
        isGoalLanding = true;
        isControlEnabled = false;
        rb.isKinematic = false;  // ゴール時の物理挙動維持

        StartCoroutine(FadeOutGlideSound());  // ゴール時にフェードアウト
    }

    private void GameOver()
    {
        // 着陸時にBGMのフェードアウトを開始
        if (bgmController != null)
        {
            bgmController.StartFadeOut();
        }

        // 操作を無効にする
        isControlEnabled = false;

        // 上昇気流と敵の生成を停止
        if (draftScript != null)
        {
            draftScript.StopDrafts();
        }
        if (PdraScript != null)
        {
            PdraScript.StopDrafts();
        }
        if (enemySpawner != null)
        {
            enemySpawner.StopSpawning();
        }
        if (BenemySp != null)
        {
            BenemySp.StopSpawning();
        }

        // GameOverUIManagerのTriggerGameOverを呼び出して、ゲームオーバー画面の処理を開始
        if (GOUI != null)
        {
            GOUI.TriggerGameOver();
        }

        // 紙飛行機の動きを止める
        rb.isKinematic = true;  // 物理挙動を止める
    }

    //各SE再生処理
    private void PlaySound(AudioClip clip)
    {
        if (audioSource != null && clip != null)
        {
            audioSource.PlayOneShot(clip);
        }
    }

    // 紙飛行機を左右に吹き飛ばす処理
    IEnumerator ThrowPlane(Vector3 direction, float force)
    {
        isThrown = true;
        isControlEnabled = false;

        // 重力を一時的に無効化
        rb.useGravity = false;

        float elapsedTime = 0f;
        float currentForce = force; // 最初は最大の力を適用

        // 紙飛行機に回転と力を加える
        while (elapsedTime < throwDuration)
        {
            // 力を加えて、時間経過とともに徐々に減少させる
            rb.AddForce(direction * currentForce, ForceMode.Acceleration);
            currentForce = Mathf.Lerp(currentForce, 0, elapsedTime / throwDuration);

            // 回転速度を抑えて、滑らかに回転させる
            rb.AddTorque(Vector3.forward * throwRotationSpeed * 0.2f * Time.deltaTime, ForceMode.Force);

            elapsedTime += Time.deltaTime;
            yield return null;
        }

        // 吹き飛ばし終了後、回転と慣性をリセット
        rb.angularVelocity = Vector3.zero; // 回転の慣性をリセット
        rb.velocity = new Vector3(0, rb.velocity.y, rb.velocity.z); // X軸の速度をリセットしてまっすぐに進むようにする

        // 紙飛行機の回転を元の状態に戻す（Z軸正方向を向くように調整）
        transform.rotation = Quaternion.LookRotation(Vector3.forward); // Z軸正方向に向ける

        // 重力を再度有効化
        rb.useGravity = true;

        // Z軸正方向に進ませるための推進力を与える
        rb.AddForce(Vector3.forward * 10f, ForceMode.VelocityChange); // Z軸正方向に強制的に進む力を与える

        // 操作を再度有効化
        yield return new WaitForSeconds(0.5f);

        // 操作を有効化
        isThrown = false;
        isControlEnabled = true;

        PlaySound(controlReturnSound); // 操作復帰の音
        StartCoroutine(FadeInGlideSound());  // 操作復帰時にフェードイン
    }

    public void StartAscending()
    {
        isAscending = true;
    }

    public void StopAscending()
    {
        isAscending = false;
    }

    // 滑空音をフェードインで再生
    private IEnumerator FadeInGlideSound()
    {
        if (!isGlidePlaying && glideClip != null)
        {
            isGlidePlaying = true;
            glideSource.Play();
            float elapsedTime = 0f;
            float startVolume = 0.2f; // フェードアウトの開始音量を0.5に設定
            glideSource.volume = startVolume;
            while (elapsedTime < glideFadeDuration)
            {
                glideSource.volume = Mathf.Lerp(0, startVolume, elapsedTime / glideFadeDuration);
                elapsedTime += Time.deltaTime;
                yield return null;
            }
            glideSource.volume = startVolume;
        }
    }

    // 滑空音をフェードアウトで停止
    private IEnumerator FadeOutGlideSound()
    {
        if (isGlidePlaying && glideSource.isPlaying)
        {
            float elapsedTime = 0f;
            float startVolume = 0.2f; // フェードアウトの開始音量を0.5に設定
            glideSource.volume = startVolume;
            while (elapsedTime < glideFadeDuration)
            {
                glideSource.volume = Mathf.Lerp(startVolume, 0, elapsedTime / glideFadeDuration);
                elapsedTime += Time.deltaTime;
                yield return null;
            }

            glideSource.volume = 0;
            glideSource.Stop();
            isGlidePlaying = false;
        }
    }
}