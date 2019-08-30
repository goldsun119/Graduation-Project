using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ItemSpawner : MonoBehaviour
{
    public GameObject itemObj0;
    public GameObject itemObj1;
    public int itemCnt = 0;

    private int itemID = 0;
    private int itemMaxCnt = 100;


    // Start is called before the first frame update
    void Start()
    {
        itemCnt = 0;
        itemID = 0;
        itemMaxCnt = 100;
    }

    // Update is called once per frame
    void Update()
    {
        if (Game.Network.NetWork.item_data.Count == 100)
        {
            if (itemCnt < itemMaxCnt)
            {
                Spawn();
                itemCnt++;
                itemID++;
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

        GameObject item;
        switch (Game.Network.NetWork.item_data[itemCnt].get_type())
        {
            case 0:
                item = Instantiate(itemObj0, spawnPos, Quaternion.identity);
                break;
            case 1:
                item = Instantiate(itemObj1, spawnPos, Quaternion.identity);
                break;
            case 2:
                item = Instantiate(itemObj0, spawnPos, Quaternion.identity);
                break;
            default:
                break;

        }

        Debug.Log("아이템 생성" + itemCnt + " " + spawnPos.x + ", " + spawnPos.y + ", " + spawnPos.z);
    }

    Vector3 RandomPosition()
    {
        Vector3 itemPosXZ = Random.insideUnitCircle * 900;
        Vector3 itemPos = new Vector3(itemPosXZ.x, 70f, itemPosXZ.y);
        
        return itemPos;
    }
}
