using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MinimapControl : MonoBehaviour
{
    public RectTransform MinimapRect;     // 인벤토리의 Rect

    public float OriginSize = 100;              // 슬롯의 사이즈.
    public float BigSize = 300;              // 슬롯의 사이즈.

    public bool Big_On = false;

    // Start is called before the first frame update
    void Start()
    {
        MinimapRect.SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, OriginSize); // 가로.
        MinimapRect.SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, OriginSize);  // 세로.

    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.M))
        {
            

            if (Big_On)
            {
                MinimapRect.SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, OriginSize); // 가로.
                MinimapRect.SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, OriginSize);  // 세로.
                Big_On = false;
            }
            else
            {
                MinimapRect.SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, BigSize); // 가로.
                MinimapRect.SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, BigSize);  // 세로.
                Big_On = true;
            }
        }

    }
}
