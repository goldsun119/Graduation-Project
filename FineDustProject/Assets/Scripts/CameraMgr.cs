using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMgr : MonoBehaviour
{
    //public Transform target;

    //[SerializeField]
    //private Vector3 camOffset;

    //[Range(0.01f, 1.0f)]
    //public float smoothFactor = 0.5f;

    //public bool LookAtPlayer = false;

    public Camera mainCam;
    public Camera subCam;

    public bool isCamMain = true;

    //public Vector3 angleR = new Vector3(20, 0, 0);

    void Start()
    {
        //camOffset = transform.position - target.position;
        MainCamOn();
    }

    // Update is called once per frame
    void Update()
    {
        if(Input.GetKeyUp(KeyCode.V))
        {
            if (isCamMain) { SubCamOn(); isCamMain = false; }
            else { MainCamOn(); isCamMain = true; }
        }
    }

    //void LateUpdate()
    //{
    //    MainCamMove();
    //    SubCamMove();
    //}

    void MainCamOn()
    {
        mainCam.enabled = true;
        subCam.enabled = false;
    }

    void SubCamOn()
    {
        mainCam.enabled = false;
        subCam.enabled = true;
    }

    void MainCamMove()
    {
        //mainCam.transform.position = target.position + new Vector3(0, 2, -2.5f);
        //mainCam.transform.rotation = Quaternion.Euler(20, 0, 0);

        //Vector3 newPos = target.position + camOffset;

        //transform.position = Vector3.Slerp(transform.position, newPos, smoothFactor);

        //if (LookAtPlayer)
        //    transform.LookAt(target);
    }

    void SubCamMove()
    {

    }
}
