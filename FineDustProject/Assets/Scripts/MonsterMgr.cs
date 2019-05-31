using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class MonsterMgr : MonoBehaviour
{
    public Transform player_tf;
    public PlayerStatus player_st;
    NavMeshAgent nav;
    //new Rigidbody rigidbody;
    MonsterSpawner Monster_Spawner;

    public int HP;
    public bool isCollision = false;
    public bool is_KnockBack = false;
    public bool is_Wait = false;
    public bool is_Tracking = false;    // tracking이 on일 때 player의 미세먼지 흡입량을 증가시키기 위해서

    public float targetRange = 50.0f;
    public float KnockBack_Time = 0.1f;
    public float Waiting_Time = 0.0f;

    void Awake()
    {
        nav = GetComponent<NavMeshAgent>();

        Monster_Spawner = GameObject.Find("MonsterSpawner").GetComponent<MonsterSpawner>();

        //rigidbody = GetComponent<Rigidbody>();
        Vector3 spawnPosition = (transform.position);
        NavMeshHit hit;
        if (NavMesh.SamplePosition(spawnPosition, out hit, 50.0f, NavMesh.AllAreas))
        {
            spawnPosition.y = hit.position.y;
            transform.position = new Vector3(spawnPosition.x, spawnPosition.y, spawnPosition.z);
            nav.Warp(new Vector3(spawnPosition.x, spawnPosition.y, spawnPosition.z));
        }
    }

    void Start()
    {
        player_tf = GameObject.FindGameObjectWithTag("Player").transform;   // 플레이어의 정보값
        player_st = player_tf.GetComponent<PlayerStatus>();
        
        targetRange = 100.0f;
        HP = 100;
    }

    // Update is called once per frame
    void Update()
    {
        if (transform.position.y < 20)
        {
            Destroy(gameObject);
            Monster_Spawner.monCnt--;
        }

        if (player_st.Ani_State_Walk_Run == PlayerStatus.ANI_TYPE.RUN)
            nav.speed = 20;
        else nav.speed = 10;


        var _distance = Vector3.Distance(player_tf.transform.position, transform.position);   // 몬스터와 플레이어 사이 거리

        // 몬스터 플레이어 따라가기
        if ((2f <= _distance) && (_distance <= targetRange) && (!is_Wait))
        {
            is_Tracking = true;
            nav.SetDestination(player_tf.transform.position);
            //Debug.Log("몬스터와 캐릭터 사이의 거리" + _distance);
        }
        else
        {
            is_Tracking = false;
            nav.SetDestination(transform.position);
            transform.LookAt(player_tf);

        }

        // 플레이어가 몬스터 공격
        if (isCollision)
        {
            if (Input.GetMouseButtonUp(0)) //0:좌 1:중 2:우
            {
                HP -= 10;
                Debug.Log("피 " + HP + "남음");
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
            Debug.Log("멧돼지 사망");
            Monster_Spawner.monCnt--;
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

    //bool CheckDistance()
    //{
    //    float distance = Vector3.Distance(player.transform.position, transform.position);
    //    Debug.Log("몬스터와 캐릭터 사이의 거리" + distance);
    //    if (distance <= targetRange) return true;

    //    return false;
    //}
}
