using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class ItemSpawn : MonoBehaviour
{
    public GameObject itemObj;

    //public float landRad = 945f;            // 섬지름 1900이지만 1890으로 잡음
    public Vector3 landCenterPos = new Vector3(1000, 0, 1000);



    public int itemCnt = 1;
    public int itemMaxCnt = 100;
    public Vector3 spawnPos;


    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (itemCnt < itemMaxCnt)
        {
            Spawn();
            itemCnt++;
            Debug.Log(itemCnt);
        }
    }

    void Spawn()
    {
        spawnPos = RandomPosition();

        if(Mathf.Exp(spawnPos.x) + Mathf.Exp(spawnPos.z) == Mathf.Exp(350))
        {
            itemCnt--;
            Debug.Log("확인중");
            return;
        }

        Debug.Log("넘어감");
        spawnPos += Vector3.up * 0.6f;

        Debug.Log(spawnPos);

        GameObject item = Instantiate(itemObj, spawnPos, Quaternion.identity);
    }

    Vector3 RandomPosition()
    {
        // 랜덤 좌표 설정하는 법
        // 1. 네비매쉬를 이용 -> 서버에서 네비매쉬 구워서 넘겨줄 수 있어야함
        // 2. 그냥 랜덤값 구해서 벡터 만들고 서버에 넘기기.  -> 너무 쉬운데

        Vector3 itemPos = Random.insideUnitSphere * 900 + landCenterPos;

        itemPos.y = 0f;

        //Debug.Log(itemPos.x + ", " + itemPos.y + ", " + itemPos.z);
        return itemPos;
        //Vector3 itemPosition = Random.insideUnitSphere * landRad + landCenterPos;

        //NavMeshHit hit;

        //NavMesh.SamplePosition(itemPosition, out hit, landRad, NavMesh.AllAreas);

        //return hit.position;
    }
}
