﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class MonsterMgr : MonoBehaviour
{
    public Transform player_tf;
    public PlayerStatus player_st;
    NavMeshAgent nav;
    Animator ani;
    //new Rigidbody rigidbody;
    //MonsterSpawner Monster_Spawner;

    public enum ANI_TYPE { IDEL, WALK, RUN, JUMP };

    public int HP;
    public bool isCollision = false;
    public bool is_KnockBack = false;
    public bool is_Wait = false;
    public bool is_Tracking = false;    // tracking이 on일 때 player의 미세먼지 흡입량을 증가시키기 위해서
    public bool is_BackToSpawn = false;

    public float targetRange = 20.0f;
    public float KnockBack_Time = 0.1f;
    public float Waiting_Time = 0.0f;

    public Vector3 Random_Position;
    public float default_velocity;

    public Vector3 spawnPosition;

    public int ID;
    public int chase_id;
    public int calculate_id;
    public int animator;
    public float dirX;
    public float dirZ;
    public Vector3 rotation;
    public Vector3 position;

    public Vector3 target_pos;

    void Awake()
    {
        nav = GetComponent<NavMeshAgent>();
        ani = GetComponent<Animator>();
        animator = (int)ANI_TYPE.IDEL;
        //Monster_Spawner = GameObject.Find("MonsterSpawner").GetComponent<MonsterSpawner>();

        //rigidbody = GetComponent<Rigidbody>();
        spawnPosition = (transform.position);
        NavMeshHit hit;
        if (NavMesh.SamplePosition(spawnPosition, out hit, 50.0f, NavMesh.AllAreas))
        {
            spawnPosition.y = hit.position.y;
            transform.position = new Vector3(spawnPosition.x, spawnPosition.y, spawnPosition.z);
            nav.Warp(new Vector3(spawnPosition.x, spawnPosition.y, spawnPosition.z));
        }

        //ID = Monster_Spawner.monID;
    }

    void Start()
    {
        //player_tf = GameObject.FindGameObjectWithTag("Player").transform;   // 플레이어의 정보값
        //Debug.Log(player_tf.gameObject.name);
        //player_st = player_tf.GetComponent<PlayerStatus>();

        targetRange = 20.0f;
        //HP = 100;
        HP = Game.Network.NetWork.monster_data[ID].get_animator();
        chase_id = Game.Network.NetWork.monster_data[ID].get_chase_id();
        calculate_id = Game.Network.NetWork.monster_data[ID].get_calculate_id();
        animator = Game.Network.NetWork.monster_data[ID].get_animator();
        dirX = Game.Network.NetWork.monster_data[ID].get_dirX();
        dirZ = Game.Network.NetWork.monster_data[ID].get_dirZ();
        rotation = Game.Network.NetWork.monster_data[ID].get_rot();
        position = Game.Network.NetWork.monster_data[ID].get_pos();
        Random_Position = position;
        spawnPosition = position;
    }

    // Update is called once per frame
    void Update()
    {
        player_tf = GameObject.FindGameObjectWithTag("Player").transform;   // 플레이어의 정보값
        Debug.Log(player_tf.gameObject.name);
        player_st = player_tf.GetComponent<PlayerStatus>();

        if (Game.Network.NetWork.monster_data.ContainsKey(ID) == false || Game.Network.NetWork.monster_data[ID].get_draw() == false || Game.Network.NetWork.monster_data[ID].get_hp()<=0)
        {
            Destroy(gameObject);
            Debug.Log("멧돼지 사망");
        }
        position = Game.Network.NetWork.monster_data[ID].get_pos();
        rotation = Game.Network.NetWork.monster_data[ID].get_rot();
        dirX = Game.Network.NetWork.monster_data[ID].get_dirX();
        dirZ = Game.Network.NetWork.monster_data[ID].get_dirZ();
        chase_id = Game.Network.NetWork.monster_data[ID].get_chase_id();
        calculate_id = Game.Network.NetWork.monster_data[ID].get_calculate_id();
        animator = Game.Network.NetWork.monster_data[ID].get_animator();
        HP = Game.Network.NetWork.monster_data[ID].get_animator();
        if (calculate_id == Game.Network.NetWork.Client_id)
        {
            if (chase_id == 0)
            {
                if (Vector3.Distance(Random_Position, spawnPosition) > 80)
                {
                    //animator = (int)ANI_TYPE.RUN;

                    Random_Position += (spawnPosition - Vector3.zero);
                }
                else
                {
                    //animator = (int)ANI_TYPE.WALK;

                    Random_Position.x += Random.Range(-0.5f, 0.5f);
                    Random_Position.z += Random.Range(-0.5f, 0.5f);
                }
                nav.SetDestination(Random_Position);

                var spawn_distance = Vector3.Distance(spawnPosition, transform.position);   // 스폰 영역 밖으로 나갔는지 확인 할 거리
                if (spawn_distance > 100)
                {
                    is_BackToSpawn = true;
                }
                if (is_BackToSpawn)
                {
                    //animator = (int)ANI_TYPE.RUN;

                    nav.SetDestination(spawnPosition);

                    if (transform.position == spawnPosition)
                        is_BackToSpawn = false;
                }


                // 플레이어가 몬스터 공격
                if (isCollision)
                {
                    if (Input.GetMouseButtonUp(0)) //0:좌 1:중 2:우
                    {
                        is_KnockBack = true;
                        KnockBack_Time = 0.1f;
                        Game.Network.NetWork.SendMonsterAttack(ID);
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

                position = transform.position;
                rotation = new Vector3(transform.eulerAngles.x, transform.eulerAngles.y, transform.eulerAngles.z);
                Game.Network.NetWork.monster_data[ID].set_animator(HP);
                Game.Network.NetWork.monster_data[ID].set_hp(HP);
                Game.Network.NetWork.monster_data[ID].set_dirX(dirX);
                Game.Network.NetWork.monster_data[ID].set_dirZ(dirZ);
                Game.Network.NetWork.monster_data[ID].set_rot(rotation);
                Game.Network.NetWork.monster_data[ID].set_pos(position);

                Game.Network.NetWork.SendMonsterInfo(ID);
            }
            else
            {
                target_pos = Game.Network.NetWork.client_data[chase_id].get_pos();
                float d = Vector3.Distance(spawnPosition, transform.position);
                if (d > 100)
                {
                    //animator = (int)ANI_TYPE.RUN;

                    nav.SetDestination(spawnPosition);
                }
                else //if (d < 100 && d > 2)
                {
                    nav.SetDestination(target_pos);

                }
                //else if (d > 2)
                //{
                //    nav.SetDestination(target_pos);
                //    nav.speed = 3;
                //    animator = (int)ANI_TYPE.WALK;
                //    ani.SetInteger("animator", 0);
                //}

                if (player_st.Ani_State == PlayerStatus.ANI_TYPE.RUN)
                {
                    nav.speed = 20;
                    animator = (int)ANI_TYPE.RUN;
                    ani.SetInteger("animator", 2);
                }
                else if (player_st.Ani_State == PlayerStatus.ANI_TYPE.WALK)
                {
                    nav.speed = 10;
                    animator = (int)ANI_TYPE.RUN;
                    ani.SetInteger("animator", 2);

                }
                else if (player_st.Ani_State == PlayerStatus.ANI_TYPE.IDEL)
                {
                    nav.speed = 0;
                    animator = (int)ANI_TYPE.IDEL;
                    ani.SetInteger("animator", 0);
                }
                else
                {
                    nav.speed = 3;
                    animator = (int)ANI_TYPE.WALK;
                    ani.SetInteger("animator", 1);
                }


                // 플레이어가 몬스터 공격
                if (isCollision)
                {
                    if (Input.GetMouseButtonUp(0)) //0:좌 1:중 2:우
                    {
                        is_KnockBack = true;
                        KnockBack_Time = 0.1f;
                        Game.Network.NetWork.SendMonsterAttack(ID);
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

                position = transform.position;
                rotation = new Vector3(transform.eulerAngles.x, transform.eulerAngles.y, transform.eulerAngles.z);
                Game.Network.NetWork.monster_data[ID].set_animator(HP);
                Game.Network.NetWork.monster_data[ID].set_hp(HP);
                Game.Network.NetWork.monster_data[ID].set_dirX(dirX);
                Game.Network.NetWork.monster_data[ID].set_dirZ(dirZ);
                Game.Network.NetWork.monster_data[ID].set_rot(rotation);
                Game.Network.NetWork.monster_data[ID].set_pos(position);

                Game.Network.NetWork.SendMonsterInfo(ID);
            }
        }
        else
        {
            transform.position = position;
            transform.rotation = Quaternion.Euler(rotation.x, rotation.y, rotation.z);

            // 플레이어가 몬스터 공격
            if (isCollision)
            {
                if (Input.GetMouseButtonUp(0)) //0:좌 1:중 2:우
                {
                    is_KnockBack = true;
                    KnockBack_Time = 0.1f;
                    Game.Network.NetWork.SendMonsterAttack(ID);
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

            position = transform.position;
            rotation = new Vector3(transform.eulerAngles.x, transform.eulerAngles.y, transform.eulerAngles.z);
            Game.Network.NetWork.monster_data[ID].set_animator(HP);
            Game.Network.NetWork.monster_data[ID].set_hp(HP);
            Game.Network.NetWork.monster_data[ID].set_dirX(dirX);
            Game.Network.NetWork.monster_data[ID].set_dirZ(dirZ);
            Game.Network.NetWork.monster_data[ID].set_rot(rotation);
            Game.Network.NetWork.monster_data[ID].set_pos(position);

            Game.Network.NetWork.SendMonsterInfo(ID);
        }
    }

    void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.CompareTag("Player"))
        {
            isCollision = true;
        }

        if (collision.gameObject.CompareTag("Doom"))
        {
            is_BackToSpawn = true;
            Game.Network.NetWork.monster_data[ID].set_chase_id(0);
        }
    }

    private void OnCollisionStay(Collision collision)
    {
        if (collision.gameObject.CompareTag("Doom"))
        {
            is_BackToSpawn = true;
            Game.Network.NetWork.monster_data[ID].set_chase_id(0);
        }
    }

    void OnCollisionExit(Collision collision)
    {
        if (collision.gameObject.CompareTag("Player"))
            isCollision = false;
    }

    void OnTriggerStay(Collider other)
    {
        Debug.Log(other.gameObject.name);
        if (other.tag == "Player")
        {
            Transform other_tf = other.gameObject.transform;
            PlayerStatus other_st = other_tf.GetComponent<PlayerStatus>();

            if (Vector3.Distance(other_st.transform.position, transform.position)
                <= Vector3.Distance(player_tf.transform.position, transform.position))
            {
                Debug.Log(player_tf.gameObject.name);
                player_tf = other_tf;
                player_st = other_st;

            }
            is_Tracking = true;
        }

        if (other.tag == "Doom")
        {
            is_BackToSpawn = true;
            Game.Network.NetWork.monster_data[ID].set_chase_id(0);
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
/*
 좌표랑 숫자를 주면 그 좌표에 숫자를 아이디로 갖는거로 생성하기
숫자에 해당하는 몬스터 삭제하기
타겟아이디가 0이고 계산아이디가 클라아이디랑 같으면 자유행동
타겟아이디가 0인데 계산아이디가 클라아이디랑 다르면 서버에서 정보 받아서 적용
타겟아이디가 0이 아닐때
타겟아이디가 클라아이디와 같으면 길찾기
캐릭터한테 맞았을때 서버로 맞았다는것만 보내기
 */
