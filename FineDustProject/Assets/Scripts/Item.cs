using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class Item : MonoBehaviour
{
    public bool isCollision = false;
    public GameObject player;
    new Rigidbody rigidbody;
    ItemSpawner Item_Spawner;

    public enum TYPE { Box, Crystal }

    public TYPE type;           // 아이템의 타입.
    public Sprite DefaultImg;   // 기본 이미지.
    public int MaxCount;        // 겹칠수 있는 최대 숫자.

    private Inventory Iv;

    public int ID;

    void Awake()
    {
        // 태그명이 "Inventory"인 객체의 GameObject를 반환한다.
        // 반환된 객체가 가지고 있는 스크립트를 GetComponent를 통해 가져온다.
        Iv = GameObject.FindGameObjectWithTag("Inventory").GetComponent<Inventory>();
        
        Item_Spawner = GameObject.Find("ItemSpawner").GetComponent<ItemSpawner>();

        rigidbody = GetComponent<Rigidbody>();
        Vector3 spawnPosition = (transform.position);
        NavMeshHit hit;
        if (NavMesh.SamplePosition(spawnPosition, out hit, 50.0f, NavMesh.AllAreas))
        {
            //Debug.Log("hit"+hit.position);
            //Debug.Log("spawn"+spawnPosition);
            spawnPosition.y = hit.position.y;
            transform.position = new Vector3(spawnPosition.x, spawnPosition.y, spawnPosition.z);
        }

        ID = Item_Spawner.itemCnt;
    }

    void Start()
    {
        MaxCount = 3;
    }

    void Update()
    {
        if (isCollision)
        {
            if (Input.GetKey(KeyCode.F))
            {
                Game.Network.NetWork.SendEatItem(ID, Game.Network.NetWork.Client_id);
                AddItem();
                gameObject.SetActive(false);
                //Item_Spawner.itemCnt--;
                //Game.Network.NetWork.item_data[].set_draw(false);
                //번호 알아서 그거 드로우 끄고 드로우상태에 따라 액티브 트루/폴스로

                Debug.Log("냠");
            }
        }

        if (Game.Network.NetWork.item_data[ID].get_draw())
        { }
        else
        {
            gameObject.SetActive(false);
        }
        if (transform.position.y < 20)
        {
            Destroy(gameObject);
        }
        
    }

    void AddItem()
    {
        // 아이템 획득에 실패할 경우.
        if (!Iv.AddItem(this))
            Debug.Log("아이템이 가득 찼습니다.");
        else // 아이템 획득에 성공할 경우.
        {
            Destroy(gameObject); // 아이템을 비활성화 시켜준다.
            //Item_Spawner.itemCnt--;
        }
    }

    void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.CompareTag("Player"))
            isCollision = true;

        //if (collision.gameObject.CompareTag("Terrain"))
        //{
        //    if (null != rigidbody)

        //        Destroy(GetComponent<Rigidbody>());
        //    Debug.Log("땅에 만난 템");
        //}

    }

    void OnCollisionExit(Collision collision)
    {
        if (collision.gameObject.CompareTag("Player"))
            isCollision = false;
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.CompareTag("Player"))
            isCollision = true;

    }

    private void OnTriggerExit(Collider other)
    {
        if (other.gameObject.CompareTag("Player"))
            isCollision = false;
    }
}
