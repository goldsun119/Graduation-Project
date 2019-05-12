using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Door : MonoBehaviour
{
    public Transform player_tf;
    public GameObject player;
    public bool isCollision = false;
    Animator animator;

    public float targetRange = 50.0f;

    // Start is called before the first frame update
    void Start()
    {
        player_tf = GameObject.FindGameObjectWithTag("Player").transform;   // 플레이어의 정보값
        animator = GetComponent<Animator>();
    }


    // Update is called once per frame
    void Update()
    {
        var _distance = Vector3.Distance(player_tf.transform.position, transform.position);   // 몬스터와 플레이어 사이 거리

        // 몬스터 플레이어 따라가기
        if (2f > _distance)
        {
            if (Input.GetMouseButtonUp(0)) //0:좌 1:중 2:우
            {
                if (animator.GetBool("Is_Open") == false)
                {
                    animator.SetBool("Is_Open", true);
                    Debug.Log("열려라 참깨");
                }
                else if (animator.GetBool("Is_Open") == true)
                    animator.SetBool("Is_Open", false);
            }
        }
        else
        {
            animator.SetBool("Is_Open", false);
        }
    }

    void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.CompareTag("Player"))
        {
            isCollision = true;
        }
    }

    void OnCollisionExit(Collision collision)
    {
        if (collision.gameObject.CompareTag("Player"))
            isCollision = false;
    }
}
