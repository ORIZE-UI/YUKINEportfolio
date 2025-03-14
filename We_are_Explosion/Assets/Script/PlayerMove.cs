using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerMove : MonoBehaviour
{
	[Tooltip("プレイヤーの移動速度の設定(既定値：5.0)")]
	public float velocity = 5.0f;
	[Tooltip("プレイヤーのジャンプ力の設定(既定値：5.0)")]
	public float jumpPower = 5.0f;
	[Tooltip("プレイヤーの壁を登る移動速度の設定(既定値：1.0)")]
	public float ladderSpeed = 1.0f;
	[Tooltip("プレイヤーが壁を上る際に回転する速度(デフォ5)")]
	public float rotationSpeed = 5.0f;
	[Tooltip("プレイヤーが壁を上る際に重力なしの時間(デフォ3)")]
	public int notGravityTime = 3;
	[Tooltip("歩けるオブジェクトのタグを設定")]
	public List<string> WalkObjTags = new() { "Grabbable" };


	[Tooltip("インタラクティブなオブジェクト(はしごなど)に触れるボタンを設定してください。")]
	public string interactButton = "Fire3"; // デフォルト(Fire3= KeyCode.JoystickButton2, KeyCode.E)

	private Rigidbody rb;
	private bool isGrounded;
	private bool isLadder;
	private bool isInteract;
	private bool isTachObj;
	private bool isNowTachObj;
	private bool isUnTachObj;
	private bool isResetTachObj;
	private bool isRotatingR;
	private bool isRotatingL;
	public string tachObject = "Untagged";  // 触っているオブジェクトのタグ
	private PlayerInput playerInput;
	private int nUnTachWait = 0;
	private int nResetWait = 0;
	private int nTachWait = 0;
	private float angleDifference;
	private float rotationThreshold = 0.1f;
	Quaternion currentRotation; // プレイヤーの初期の回転角度保存用
	Quaternion rotationNow;
	Quaternion targetRotation;
	Vector3 forward;

	public Animator animator;

	private bool isFirst = false; // 初回時のみデバッグログを出力するためのフラグ


	////////////////
	// 使わない変数
	////////////////
	Quaternion rotationDelta;
	Quaternion MaxRotationRight;
	Quaternion MaxRotationLeft;
	Vector3 PlayerRight;
	Vector3 PlayerUp;
	


	// Start is called before the first frame update
	void Start()
	{
		// Rigidbodyを取得
		rb = GetComponent<Rigidbody>();
		// 最初は着地をしていない状態
		isGrounded = false;
		// 最初ははしごに触れていない状態
		isLadder = false;
		// 最初は何もインタラクトをしていない状態
		isInteract = false;
		// 最初はリセットされてない
		isResetTachObj = false;
		// 最初は回転してない
		isRotatingR = false;
		isRotatingL = false;
		// 最初は壁に触っていないからタイマーは0
		nResetWait = 0;
		nTachWait = 0;
		// PlayerInputを取得
		playerInput = GetComponent<PlayerInput>();

		//animator = GetComponent<Animator>(); // Animatorコンポーネントを取得
		animator = GetComponentInChildren<Animator>();	// 子供からとってこい親にないもん搾取するな
	}

	// Update is called once per frame
	void Update()
	{
		// 左右の移動
		//float x = Input.GetAxis("Horizontal");
		float x = playerInput.actions["Move"].ReadValue<Vector2>().x;

		// 上下の移動
		//float y = Input.GetAxis("Vertical");
		float y = playerInput.actions["Move"].ReadValue<Vector2>().y;


		if (Mathf.Abs(x) > 0.1f || Mathf.Abs(y) > 0.1f)
		{
			if (Mathf.Abs(x) > 0.1f)
			{
				animator.SetFloat("Speed", Mathf.Abs(x)); // 修正
			}
			if (Mathf.Abs(y) > 0.1f)
			{
				animator.SetFloat("Speed", Mathf.Abs(y)); // 修正
			}
		}
		else
		{
			animator.SetFloat("Speed", 0f); // 修正
		}


		var move = playerInput.actions["Move"].ReadValue<Vector2>();
		var jump = playerInput.actions["Jump"].IsPressed();
		var interact = playerInput.actions["Interactive"].IsPressed();

		PlayerRight = transform.right;
		PlayerUp = transform.up;

		if (x != 0 && isTachObj == false)
		{
			if (isTachObj != true)
			{
				// 回転角度を計算
				float targetAngle = x > 0 ? 0 : 180;

				// 回転を設定
				transform.rotation = Quaternion.Euler(0, targetAngle, 0);
			}
		}
		// プレイヤーの向きを取得
		forward = transform.forward;
		// 現在の向きを取得
		if (forward == Vector3.back)
		{
			// 左向き
			isRotatingL = true;
			// 左向いたら右のフラグを消す
			isRotatingR = false;
			// 左向きの回転前の向きを格納
			currentRotation = Quaternion.Euler(0, 180, 0);
		}
		else if (forward == Vector3.forward)
		{
			// 右向き
			isRotatingR = true;
			// 右向いたら左のフラグを消す
			isRotatingL = false;
			// 右向きの回転前の向きを格納
			currentRotation = Quaternion.Euler(0, 0, 0);
		}

		/////////////////////////////////////////
		// 着地しているかどうかを判定
		/////////////////////////////////////////
		if (isGrounded == true && isTachObj == false)
		{
			animator.SetBool("Isjump", false);
			// Jumpボタンが押されたら
			if (Input.GetButton("Jump") == true || jump == true)
			{
				animator.SetBool("Isjump", true);

				// ジャンプの方向を上向きのベクトルに設定
				Vector3 jump_vector = Vector3.up;
				// ジャンプの速度を計算
				Vector3 jump_velocity = jump_vector * jumpPower;

				// 上向きの速度を設定
				rb.velocity = jump_velocity;
				// 着地していない状態にする
				isGrounded = false;
			}
		}

		/////////////////////////////////////////
		// 壁を歩く処理
		/////////////////////////////////////////
		if (isTachObj == true && nTachWait < 0)
		{
			animator.SetBool("Isjump", false);
			// 現在の回転を取得
			rotationNow = transform.rotation;

			if (isRotatingL)
			{// 左向き
			 // Y軸を180度、Z軸を90度回転させる
				targetRotation = Quaternion.Euler(0, 180, 90);
				// 現在の回転と目標の回転の差を計算
				angleDifference = Quaternion.Angle(transform.rotation, targetRotation);
				if (isResetTachObj == false)
				{
					// 壁に吸い付くように移動させる
					rb.velocity = new Vector3(-5, y * ladderSpeed * 2.0f, 0);
				}
			}
			else if (isRotatingR)
			{// 右向き
			 // Z軸を90度回転させる
				targetRotation = Quaternion.Euler(0, 0, 90);
				// 現在の回転と目標の回転の差を計算
				angleDifference = Quaternion.Angle(transform.rotation, targetRotation);
				if (isResetTachObj == false)
				{
					// 壁に吸い付くように移動させる
					rb.velocity = new Vector3(5, y * ladderSpeed * 2.0f, 0);
				}
			}

			// 角度の差が閾値以下なら回転完了
			if (angleDifference < rotationThreshold)
			{
				transform.rotation = targetRotation; // 目標の回転を設定
			}

			// スムーズに回転させる
			transform.rotation = Quaternion.Slerp(transform.rotation, targetRotation, rotationSpeed * Time.deltaTime);
			Debug.Log("回転中");

			if (nResetWait > 20)
			{
				// `Jump`ボタンが押されたら
				if (Input.GetButton("Jump") == true)
				{
					// ポジション管理
					if (isRotatingL)
					{
						// 壁から離れるように移動させる
						rb.velocity = new Vector3(7, y * ladderSpeed * 2.0f, 0);
					}
					else if (isRotatingR)
					{
						// 壁から離れるように移動させる
						rb.velocity = new Vector3(-7, y * ladderSpeed * 2.0f, 0);
					}
					// フラグ管理
					if (isResetTachObj == false)
					{
						// フラグをおろす
						isTachObj = false;
						isUnTachObj = false;
						rb.useGravity = true;
						// リセットフラグを上げる
						isResetTachObj = true;
					}
				}
				nResetWait = 21;
			}
			else
			{
				nResetWait++;
			}
		}
		else
		{
			// 壁から離れた場合は、元の回転に戻す
			targetRotation = currentRotation;
			rb.useGravity = true;

			// スムーズに回転させる
			transform.rotation = Quaternion.Slerp(transform.rotation, targetRotation, rotationSpeed * Time.deltaTime);
			//Debug.Log("回転中");

			if (nResetWait > 20)
			{
				nTachWait = 21;
			}
			else
			{
				nTachWait--;
			}
			nResetWait = 0;
		}



		if (isUnTachObj == true && isNowTachObj == false)
		{
			nUnTachWait++;
			if (nUnTachWait > notGravityTime)
			{// notGravityTimeは3～5ぐらいがいい
				nUnTachWait = 0;
				// 壁から離れたら
				isTachObj = false;
				isUnTachObj = false;
				rb.useGravity = true;
				isResetTachObj = false;
			}
		}
		else if (isNowTachObj == true)
		{
			nUnTachWait = 0;
		}

		// はしごが近くにあるときにVerticalキーが押されたら
		// 相手のタグがLadderかどうかを判定
		if (isLadder == true)
		{// 梯子
			if (Input.GetButtonDown("Fire3") || interact == true)
			{
				if (isInteract == false)
				{
					// 重力をなくす
					rb.useGravity = false;

					isInteract = true;
				}
				else
				{
					// 重力を元に戻す
					rb.useGravity = true;

					isInteract = false;
				}
			}
			if (isInteract)
			{// 梯子移動
				rb.velocity = new Vector3(x * velocity, y * ladderSpeed, 0);
			}
		}

		/////////////////////////////////////////
		// 移動方向を設定
		/////////////////////////////////////////
		if (playerInput == null)
		{
			if (isTachObj != true)
			{// 通常移動
				rb.velocity = new Vector3(x * velocity, rb.velocity.y, 0);
			}
		}
		else
		{
			// 初回のみデバッグログを出力
			if (isFirst == false)
			{
				Debug.Log("Using PlayerInput");
				isFirst = true;
			}

			if (isTachObj != true)
			{
				rb.velocity = new Vector3(move.x * velocity, rb.velocity.y, 0);
			}
		}

	}

	/////////////////////////////////////////
	//
	// 以下接触判定時呼び出しなどの関数
	//
	/////////////////////////////////////////
	// 壁判定
	// 壁接触時
	/////////////////////////////////////////
	void OnCollisionEnter(Collision other)
	{
		// 接地している状態にする
		isGrounded = true;

		// 触れたオブジェが歩ける壁かどうか見る
		foreach (var WalkObjTag in WalkObjTags) // 右を左に格納して実行
		{
			if (other.gameObject.CompareTag(WalkObjTag))
			{
				// 歩ける壁ならフラグを立てる
				isTachObj = true;


				isNowTachObj = true;
				rb.useGravity = false;
			}
		}
	}
	/////////////////////////////////////////
	// 壁判定
	// 壁から離れた時
	/////////////////////////////////////////
	void OnCollisionExit(Collision other)
	{
		// 離れたオブジェが歩ける壁かどうか見る
		foreach (var WalkObjTag in WalkObjTags) // 右を左に格納して実行
		{
			if (other.gameObject.CompareTag(WalkObjTag))
			{
				isUnTachObj = true;
				isNowTachObj = false;


				// 着地していない状態にする
				isGrounded = false;
			}
		}
	}

	/////////////////////////////////////////
	// はしご
	// はしご接触時判定
	/////////////////////////////////////////
	void OnTriggerEnter(Collider other)
	{
		// 相手がLadderかどうかを判定
		if (other.gameObject.tag == "Ladder")
		{
			// はしごに触れている状態にする
			isLadder = true;
			//rb = GetComponent<Rigidbody>();

			// 重力を0にする
			//rb.useGravity = false;

			Debug.Log("はしごに触れました。");

		}
	}
	/////////////////////////////////////////
	// はしご
	// はしごから離れた判定
	/////////////////////////////////////////
	private void OnTriggerExit(Collider other)
	{
		// 相手がLadderだったかどうかを判定
		if (other.gameObject.tag == "Ladder")
		{
			// はしごから離れた状態にする
			isLadder = false;
			// 重力を元に戻す
			rb.useGravity = true;
			// 梯子から離れたらインタラクトしたアクションの終了
			isInteract = false;

			Debug.Log("はしごから離れました。");
		}
	}

	public void OnPlayerMove(InputAction.CallbackContext context)
	{
		Debug.Log("PlayerMove");
	}

	public void OnJump(InputAction.CallbackContext context)
	{
		Debug.Log("Jump");
	}

	public void OnInteract(InputAction.CallbackContext context)
	{
		Debug.Log("Interact");
	}
}
