using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ItemSpawner : MonoBehaviour
{
    public GameObject itemObj;

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
        if(itemCnt < itemMaxCnt)
        {
            Spawn();
            itemCnt++;
            Debug.Log(itemCnt);
        }
    }

    void Spawn()
    {
        spawnPos = RandomPosition();

        if((spawnPos.x * spawnPos.x) + (spawnPos.z * spawnPos.z) <= (350*350))
        {
            itemCnt--;
            Debug.Log("돔 안임");
            return;
        }

        spawnPos += Vector3.up * 0.6f;

        GameObject item = Instantiate(itemObj, spawnPos, Quaternion.identity);
    }

    Vector3 RandomPosition()
    {
        Vector3 itemPosXZ = Random.insideUnitCircle * 900;
        Vector3 itemPos = new Vector3(itemPosXZ.x, 0f, itemPosXZ.y);

        Debug.Log(itemPos.x + ", " + itemPos.y + ", " + itemPos.z);

        return itemPos;
    }
}
