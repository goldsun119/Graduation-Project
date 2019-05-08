using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

using Game.Protocol;

public class PlayerSet : MonoBehaviour
{
    private InputField ip;
    public GameObject playerObj;
    public GameObject[] players;
    public PlayerStatus playerStatus;

    public int num = 1;

    private void Awake()
    {
        CreateAllPlayer();
    }

    void CreateAllPlayer()
    {
        players = new GameObject[Game.Protocol.Protocol.MAX_USER];

        for(int i = 0; i<Game.Protocol.Protocol.MAX_USER; ++i)
        {
            Vector3 spawnPos = Vector3.zero;
            if (i == 1)
            {
                spawnPos = new Vector3(-1, 0, -6);
            }
            else if (i == 2)
            {
                spawnPos = new Vector3(1, 0, -6);
            }
            else if (i == 3)
            {
                spawnPos = new Vector3(-1, 0, -7);
            }
            else if (i == 4)
            {
                spawnPos = new Vector3(1, 0, -7);
            }
            players[i] = GameObject.Instantiate(playerObj, spawnPos, Quaternion.identity) as GameObject;
            players[i].SetActive(false);
        }
    }

    public void Spawn()
    {
        //for(int i = 0; i< Game.Network.NetWork.client_data.Count;++i)
        //{
        //    if (i != 0 && i != Game.Network.NetWork.Client_id)
        //    {
        //        int other_id = Game.Network.NetWork.client_data[i].get_id();
        //        players[other_id].SetActive(true);
        //    }
        //}
        for (int i = 0; i < Game.Protocol.Protocol.MAX_USER; ++i)
        {
            //if (i != 0 && i != Game.Network.NetWork.Client_id)
            {
                players[i].SetActive(true);
            }
        }
        //playerStatus = FindObjectOfType<PlayerStatus>();

        //playerStatus.ID = num;
    }

}