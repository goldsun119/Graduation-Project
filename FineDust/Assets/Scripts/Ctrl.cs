using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ctrl : MonoBehaviour
{

    CameraMgr camMgr;

    // Use this for initialization
    void Start()
    {

        camMgr = GetComponent<CameraMgr>();

    }

    // Update is called once per frame
    void Update()
    {

        if (Input.GetKeyUp(KeyCode.V))
        {
            if (camMgr.isCamMain) { camMgr.SubCamOn(); camMgr.isCamMain = false; }
            else { camMgr.MainCamOn(); camMgr.isCamMain = true; }
        }

    }
}
