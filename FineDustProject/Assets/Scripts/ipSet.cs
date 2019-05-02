using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ipSet : MonoBehaviour
{
    private InputField ip;
    public GameObject playerObj;
    public PlayerStatus playerStatus;

    public int num = 1;

   public void spawn()
    {
        Vector3 spawnPos;

        if (num == 1)
        {
            spawnPos = new Vector3(-1, 0, -6);
            num++;

        }
        else if (num == 2)
        {
            spawnPos = new Vector3(1, 0, -6);
            num++;
        }
        else if (num == 3)
        {
            spawnPos = new Vector3(-1, 0, -7);
            num++;
        }
        else if (num == 4)
        {
            spawnPos = new Vector3(1, 0, -7);
            num++;
        }
        else
            return;

        //spawnPos += Vector3.up * 0.6f;

        GameObject player = Instantiate(playerObj, spawnPos, Quaternion.identity);

        playerStatus = FindObjectOfType<PlayerStatus>();

        playerStatus.ID = num;
    }

}
