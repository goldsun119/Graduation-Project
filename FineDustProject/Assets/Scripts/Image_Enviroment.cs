using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Image_Enviroment : MonoBehaviour
{
    GameObject playerObj;
    PlayerStatus player;

    private Image EL_Image;
    public Sprite Default_img;
    public Sprite Level1_img;
    public int Enviroment_Level;

    // Start is called before the first frame update
    void Start()
    {
        string a = "Player(" + Game.Network.NetWork.Client_id.ToString() + ")";
        playerObj = GameObject.Find("Players").transform.Find(a).gameObject;
        player = playerObj.GetComponent<PlayerStatus>();
        EL_Image = GetComponent<Image>();
        Image_resizeing();
    }

    // Update is called once per frame
    void Update()
    {
        switch (player.Enviroment_Level)
        {
            case 0:
                EL_Image.sprite = Default_img;  // 투명
                //RenderSettings.fog = false;
                RenderSettings.fogMode = FogMode.Linear;
                RenderSettings.fogDensity = 0.04f;
                //RenderSettings.fogStartDistance = 0.0f;
                //RenderSettings.fogEndDistance = 700.0f;

                break;
            case 1:
                EL_Image.sprite = Level1_img;   // 1단계
                //RenderSettings.fog = true;
                RenderSettings.fogMode = FogMode.Exponential;

                break;
            default:
                EL_Image.sprite = Default_img;
                //RenderSettings.fog = true;
                RenderSettings.fogMode = FogMode.Exponential;
                
                break;
        }
    }

    void Image_resizeing()
    {
        Vector2 myVector = new Vector2(Screen.width, Screen.height);
        transform.gameObject.GetComponent<RectTransform>().sizeDelta = myVector;
    }
}
