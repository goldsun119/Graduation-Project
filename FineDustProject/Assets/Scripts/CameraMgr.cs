using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMgr : MonoBehaviour
{
    
    public Camera Third_Cam;
    public Camera First_Cam;
    public Camera Isometric_Cam;
    public Camera MiniMap_Cam;
    public Camera NPC_Cam;

    public bool NPC_On;
    public int Camera_Num;

    // player 넣고 플레이어 받아와서 카메라주기?

    private GameObject playerObj;
    private PlayerStatus player;
    private float ver;
    private float mouseSensitivity = 3f;
    private float upDownRange = 13;


    void Start()
    {
        string a = "Player(" + Game.Network.NetWork.Client_id.ToString() + ")";
        playerObj = GameObject.Find("Players").transform.Find(a).gameObject;
        player = playerObj.GetComponent<PlayerStatus>();
        
        ThirdCamOn();
        Camera_Num = 0;
        NPC_On = false;
        NPC_Cam.enabled = false;

    }

    // Update is called once per frame
    void Update()
    {
        string a = "Player(" + Game.Network.NetWork.Client_id.ToString() + ")";
        playerObj = GameObject.Find("Players").transform.Find(a).gameObject;
        player = playerObj.GetComponent<PlayerStatus>();

        if (Input.GetKeyUp(KeyCode.V))
        {
            Camera_Num += 1;
            Camera_Num = Camera_Num % 3;
            //Debug.Log(Camera_Num);
        }

        MinimapCamMove();
        if (!NPC_On)
        switch (Camera_Num)
        {
            case 0:
                ThirdCamOn();
                break;
            case 1:
                FirstCamOn();
                break;
            case 2:
                IsometricCamOn();
                break;
            default:
                IsometricCamOn();
                break;
        }


    }


    void ThirdCamOn()
    {
        Third_Cam.enabled = true;
        First_Cam.enabled = false;
        Isometric_Cam.enabled = false;

        ThirdCamMove();
    }

    void FirstCamOn()
    {
        First_Cam.enabled = true;
        Third_Cam.enabled = false;
        Isometric_Cam.enabled = false;

        Cursor.lockState = CursorLockMode.Locked;
        
        //FirstCamMouseMove();
        FirstCamMove();
    }

    void IsometricCamOn()
    {
        Isometric_Cam.enabled = true;
        Third_Cam.enabled = false;
        First_Cam.enabled = false;

        Cursor.lockState = CursorLockMode.None; //: 일반 마우스 커서
        IsometricCamMove();
    }

    public void NPCCamOn(GameObject NPC)
    {
        NPC_On = true;

        Isometric_Cam.enabled = false;
        Third_Cam.enabled = false;
        First_Cam.enabled = false;
        NPC_Cam.enabled = true;

        Cursor.lockState = CursorLockMode.None;
        NPCCamMove(NPC);
    }

    public void NPCCamOff(GameObject NPC)
    {
        NPC_On = false;
        NPC_Cam.enabled = false;
        switch (Camera_Num)
        {
            case 0:
                ThirdCamOn();
                break;
            case 1:
                FirstCamOn();
                break;
            case 2:
                IsometricCamOn();
                break;
            default:
                IsometricCamOn();
                break;
        }
    }

    void ThirdCamMove()
    {
        Quaternion rotation = Quaternion.Euler(player.rotation.x, player.rotation.y, player.rotation.z);
        Third_Cam.transform.rotation = Quaternion.Euler(player.rotation.x + 15, player.rotation.y, player.rotation.z);
        Third_Cam.transform.position = rotation * new Vector3(0, 4.25f, -5.5f) + player.position;
    }

    void FirstCamMove()
    {

        float hor = Input.GetAxis("Mouse X");
        player.transform.Rotate(Vector3.up * mouseSensitivity * hor);
        
        First_Cam.transform.position = new Vector3(player.position.x, player.position.y + 1.4f, player.position.z);
        First_Cam.transform.rotation = Quaternion.Euler(player.rotation.x, player.rotation.y, player.rotation.z);

        ver += Input.GetAxis("Mouse Y");
        ver = Mathf.Clamp(ver, -upDownRange, upDownRange);
        First_Cam.transform.Rotate(Vector3.left * mouseSensitivity * ver);

    }

    void MinimapCamMove()
    {
        Quaternion rotation = Quaternion.Euler(player.rotation.x, player.rotation.y, player.rotation.z);
        MiniMap_Cam.transform.rotation = Quaternion.Euler(90.0f, player.rotation.y, 0.0f);
        MiniMap_Cam.transform.position = new Vector3(player.position.x, 200.0f, player.position.z);
    }

    void IsometricCamMove()
    {
        Isometric_Cam.transform.position = new Vector3(player.position.x - 5, player.position.y + 10f, player.position.z - 5);
        
    }

    void NPCCamMove(GameObject NPC)
    {
        NPC_Cam.transform.rotation = Quaternion.Euler(NPC.transform.rotation.x + 15, NPC.transform.rotation.y + 180, NPC.transform.rotation.z);
        NPC_Cam.transform.position = new Vector3(NPC.transform.position.x, NPC.transform.position.y + 1.4f, NPC.transform.position.z + 2.0f);
    }

    void FirstCamMouseMove()
    {
        //좌우 회전
        float hor = Input.GetAxis("Mouse X");
        player.transform.Rotate(Vector3.up * mouseSensitivity * hor);

        //상하 회전
        ver += Input.GetAxis("Mouse Y");
        ver = Mathf.Clamp(ver, -upDownRange, upDownRange);
        First_Cam.transform.Rotate(Vector3.left * mouseSensitivity * ver);
    }

}
