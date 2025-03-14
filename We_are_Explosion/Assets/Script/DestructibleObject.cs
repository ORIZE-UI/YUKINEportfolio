using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[Tooltip("破壊可能なオブジェクトのスクリプト")]
public class DestructibleObject : MonoBehaviour
{
	[Tooltip("爆発力")]
	public float explosionForce = 100.0f;

	// ProBuilderオブジェクトを吹き飛ばす
	private Rigidbody rb;
	private MeshCollider meshCollider;
	private GameObject player;

	// MeshColliderが使えないときはBoxColliderを使う
	private BoxCollider boxCollider;


	// 破壊済みか
	public bool IsDestroyed { get;set; } = false;

	public int nDeleteWait = 0;

    // Start is called before the first frame update
    void Start()
    {
        rb = GetComponent<Rigidbody>();

		
		meshCollider = GetComponent<MeshCollider>();

		// meshColliderもしくはboxCollider
		if (meshCollider == null)
		{
			boxCollider = GetComponent<BoxCollider>();
		}


		player = GameObject.FindWithTag("Player");

    }

    // Update is called once per frame
    void Update()
    {
		player = GameObject.FindWithTag("Player");

		if (rb.position.y < -100.0f)
		{
			Destroy(gameObject);
		}
		if (IsDestroyed)
		{
			nDeleteWait++;
			if (nDeleteWait > 50)
			{
				Destroy(gameObject);
			}
		}
    }

	private void OnCollisionEnter(Collision collision)
	{
		Debug.Log("hit object" + collision.gameObject.name + ":" +  collision.collider);
		// プレイヤーのSphereColliderに触れたら
		if (IsDestroyed != true)
		{
			// プレイヤーのSphereColliderでない場合
			if (collision.collider is not SphereCollider)
			{
				return;
			}

			
			rb.isKinematic = false;
			// RigitdbodyのFreezeを解除
			rb.constraints = RigidbodyConstraints.None;
			rb.constraints = RigidbodyConstraints.FreezePositionZ;
			// SphereColliderが当たった方向に力を加える
			Vector3 direction = collision.transform.position - transform.position;
			direction.z = 0;
			rb.AddForce(direction.normalized * explosionForce);

			if (meshCollider != null)
			{
				meshCollider.excludeLayers = LayerMask.GetMask("PlayerLayer");
			}
			else
			{
				 boxCollider.excludeLayers = LayerMask.GetMask("PlayerLayer");
			}

			// 破壊済みにする
			IsDestroyed = true;
		}

	}


}
