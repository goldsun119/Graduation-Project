using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class MonsterMgr : MonoBehaviour
{
    public Transform player_tf;
    NavMeshAgent nav;

    public int HP;
    public bool isCollision = false;
    public bool is_KnockBack = false;
    public bool is_Wait = false;

    public float targetRange = 50.0f;
    public float KnockBack_Time = 0.1f;
    public float Waiting_Time = 0.0f;

    void Start()
    {
        player_tf = GameObject.FindGameObjectWithTag("Player").transform;
        nav = GetComponent<NavMeshAgent>();
        nav.speed = 1.0f;
        targetRange = 50.0f;
        HP = 100;
    }

    // Update is called once per frame
    void Update()
    {
        player_tf = GameObject.FindGameObjectWithTag("Player").transform;

        // 몬스터 플레이어 따라가기
        if ( (Vector3.Distance(player_tf.position, transform.position) <= targetRange) && (!is_Wait))
        {
            nav.SetDestination(player_tf.position);
            float distance = Vector3.Distance(player_tf.position, transform.position);
            Debug.Log("몬스터와 캐릭터 사이의 거리" + distance);
        }
        else
        {
            nav.SetDestination(transform.position);
        }

        // 플레이어가 몬스터 공격
        if (isCollision)
        {
            if (Input.GetMouseButtonUp(0)) //0:좌 1:중 2:우
            {
                HP -= 10;
                Debug.Log("피 " + HP + "만큼 남아따");
                is_KnockBack = true;
                KnockBack_Time = 0.1f;
            }
        }

        if (is_KnockBack)
        {
            transform.Translate((Vector3.back) * KnockBack_Time);
            KnockBack_Time += Time.deltaTime;
            if (KnockBack_Time > 0.3)
            {
                is_KnockBack = false;
                is_Wait = true;
                Waiting_Time = 0.0f;
            }
        }

        if (is_Wait)
        {
            Waiting_Time += Time.deltaTime;
            if (Waiting_Time > 0.5) is_Wait = false;
        }


        if (HP == 0)
        {
            Destroy(gameObject);
            Debug.Log("나주거");
        }
        
    }

    void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.CompareTag("Player"))
        {
            isCollision = true;
        }
    }

    //bool CheckDistance()
    //{
    //    float distance = Vector3.Distance(player.transform.position, transform.position);
    //    Debug.Log("몬스터와 캐릭터 사이의 거리" + distance);
    //    if (distance <= targetRange) return true;

    //    return false;
    //}
}
