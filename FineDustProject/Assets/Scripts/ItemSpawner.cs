using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ItemSpawner : MonoBehaviour
{
    public GameObject itemObj;

    public int itemCnt = 1;
    public int itemMaxCnt = 100;


    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Game.Network.NetWork.item_data.Count == 100)
        {
            if (itemCnt <= itemMaxCnt)
            {
                Spawn();
                itemCnt++;
                Debug.Log(itemCnt);
            }
        }
    }

    void Spawn()
    {
        Vector3 spawnPos;

        //spawnPos = RandomPosition();
        spawnPos = Game.Network.NetWork.item_data[itemCnt].get_pos();

        if ((spawnPos.x * spawnPos.x) + (spawnPos.z * spawnPos.z) <= (350 * 350))
        {
            itemCnt--;
            Debug.Log("돔 안임");
            return;
        }

        //spawnPos += Vector3.up * 0.6f;

        GameObject item = Instantiate(itemObj, spawnPos, Quaternion.identity);

        Debug.Log("아이템 생성" + itemCnt + " " + spawnPos.x + ", " + spawnPos.y + ", " + spawnPos.z);
    }

    Vector3 RandomPosition()
    {
        Vector3 itemPosXZ = Random.insideUnitCircle * 900;
        Vector3 itemPos = new Vector3(itemPosXZ.x, 70f, itemPosXZ.y);
        
        return itemPos;
    }
}
