using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMgr : MonoBehaviour
{
    public Camera mainCam;
    public Camera subCam;

    public bool isCamMain = true;

    void Start()
    {
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
}
