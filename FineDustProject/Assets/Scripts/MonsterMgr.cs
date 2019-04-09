using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class MonsterMgr : MonoBehaviour
{
    GameObject player;
    NavMeshAgent nav;

    public int HP;
    public bool isCollision = false;

    public float targetRange = 5.0f;

    void Start()
    {
        player = GameObject.FindGameObjectWithTag("Player");
        nav = GetComponent<NavMeshAgent>();
        nav.speed = 1.0f;

        HP = 100;
    }

    // Update is called once per frame
    void Update()
    {
        // 몬스터 플레이어 따라가기
        if (CheckDistance())
        {
            nav.SetDestination(player.transform.position);
            Debug.Log(player.transform.position);
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
            }
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

    bool CheckDistance()
    {
        float distance = player.transform.position.magnitude;

        if (distance <= targetRange) return true;

        return false;
    }
}
