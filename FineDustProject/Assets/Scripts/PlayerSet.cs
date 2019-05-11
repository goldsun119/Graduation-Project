using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

using Game.Protocol;

public class PlayerSet : MonoBehaviour
{
    public GameObject playerObj;
    public static PlayerStatus playerStatus;
    
    public void Spawn()
    {
        for(int i =1; i<= Game.Network.NetWork.Client_id; ++i)
        {
            string a = "Player(" + i.ToString() + ")";

            playerObj = GameObject.Find("Players").transform.Find(a).gameObject;
            playerStatus = playerObj.GetComponent<PlayerStatus>();

            if (i == playerStatus.ID)
                playerObj.SetActive(true);
        }
    }
}