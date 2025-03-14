using UnityEngine;

public class PlayerMovement : MonoBehaviour
{
	public float speed = 5f; // プレイヤーの移動速度
	public float jumpForce = 8f; // ジャンプの強さ
	public LayerMask groundLayers; // 地面レイヤー
	public Transform groundCheck; // 地面チェック用のTransform
	public float groundCheckRadius = 0.2f; // 地面チェック用の円の半径

	private Rigidbody rb;


	void Start()
	{
		rb = GetComponent<Rigidbody>();
		if (rb == null)
		{
			Debug.LogError("Rigidbodyが見つかりません。Rigidbodyコンポーネントを追加してください。");
		}
	}

	void Update()
	{
		// WASDキーでの移動
		Vector3 moveDirection = Vector3.zero;
		if (Input.GetKey(KeyCode.W))
		{
			moveDirection += Vector3.forward;
		}
		if (Input.GetKey(KeyCode.S))
		{
			moveDirection += Vector3.back;
		}
		if (Input.GetKey(KeyCode.A))
		{
			moveDirection += Vector3.left;
		}
		if (Input.GetKey(KeyCode.D))
		{
			moveDirection += Vector3.right;
		}
		moveDirection.Normalize();
		transform.Translate(moveDirection * speed * Time.deltaTime, Space.World);

		if ( Input.GetKeyDown(KeyCode.Space))
		{
			rb.AddForce(Vector3.up * jumpForce, ForceMode.Impulse);
		}
	}

	
}
