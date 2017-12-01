#include <Windows.h>
#pragma comment(lib,"winmm.lib")

#include <tchar.h>

#include"Direct3D.h"

#include "Sprite.h"
#include "Texture.h"

#include "DirectInput.h"

#include"time.h"
#include"Player.h"
#include"Shadow.h"
#include"Map.h"


#define screenWidth  800
#define screenHeight 600
#define Pixel 96
#define PI 3.1415

//ウィンドウプロシージャ
LRESULT CALLBACK WndPrc
(
	HWND hWnd,	//ウィンドウのハンドル
	UINT msg,	//メッセージ
	WPARAM wParam,
	LPARAM lParam
)
{
	//メッセージ処理のための関数
	//DispatchMessageによって呼び出される

	//ウィンドウクラスの登録時に
	//各ウィンドウに設定される

	//アプリケーションがメッセージを
	//取得したら呼び出される

	//メッセージの例

	//WM_DESTROY ウィンドウが破棄された
	//			 ×ボタンが押された時など
	//WM_MOVE    ウィンドウが移動
	//WM_SIZE    ウィンドウサイズの変更
	//等

	//特別な処理を必要とする場合
	//または自分で独自の処理を定義したい場合
	//ここでメッセージの種類に応じた
	//処理を行う

	//取りあえず WM_DESTROYが来た時の
	//終了処理のみ定義

	switch (msg)
	{
		case WM_DESTROY:

			PostQuitMessage(0);

			//メッセージキューに
			//新しくWM_QUITメッセージを送る

			//いずれメッセージキューから
			//取り出され
			//メインループが終了する
			//(メインループの終了条件を
			//そのように作る)
			break;
	}

	//独自の処理を行ったら
	//デフォルトのウィンドウプロシージャを実行

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//ウィンドウ設定の名前
static const TCHAR* WC_BASIC = _T("Basic");

//作りたいウィンドウの設定を
//登録する関数
HRESULT RegistClassEx(HINSTANCE hInstance)
{
	//ウィンドウの設定を保存する構造体
	WNDCLASSEX wcex;
	
	//wcexの先頭アドレスから
	//WNDCLASSEXの長さ分のメモリを
	//全て0で埋める
	memset(&wcex, 0, sizeof(WNDCLASSEX));
	//つまりはwcexが使用しているメモリ領域を
	//全て0で初期化する処理

	//ウィンドウの設定

	//構造体のサイズ
	wcex.cbSize = sizeof(WNDCLASSEX);

	//ウィンドウの設定情報に追加する捕捉を付けないので
	//0で固定
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	//Windowのスタイル
	//それぞれがビット単位のフラグであるため
	//オア演算で合成したものがスタイルの最終的な設定
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	//CS_HREDRAW 横サイズが変わったときウィンドウ全体の再描画
	//   VREDRAW 縦サイズが変わったときウィンドウ全体の再描画
	//   DBLCLKS ダブルクリックが発生したことをウィンドウに通知

	//WinMainのインスタンスハンドル
	wcex.hInstance = hInstance;

	//ウィンドウプロシージャのアドレス
	//関数ポインタの設定
	//ウィンドウプロシージャをOSが呼び出せるようにする
	wcex.lpfnWndProc = (WNDPROC)WndPrc;

	//背景色
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	//メニュー　なし
	wcex.lpszMenuName = NULL;

	//ウィンドウクラスの名前　このウィンドウ設定の名前
	wcex.lpszClassName = WC_BASIC;

	//アイコン　デフォルトのを使う
	wcex.hIcon = (HICON)LoadImage(NULL,
		MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON,
		0, 
		0, 
		LR_DEFAULTSIZE | LR_SHARED);
	wcex.hIconSm = NULL;

	//カーソル デフォルトのものを使う
	wcex.hCursor= (HCURSOR)LoadImage(NULL,
		MAKEINTRESOURCE(IDC_ARROW),
		IMAGE_CURSOR,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED);

	//設定したウィンドウクラスを登録
	//登録したウィンドウ設定でないと
	//ウィンドウを作ることができない
	return RegisterClassEx(&wcex); 
	//HRESULT型の実行結果が戻される
}


//登録した設定を基にウィンドウを作成する
HRESULT MakeWindow
(HINSTANCE hInstance,//WinMainの識別子
	HWND &refHWnd,	//ウィンドウの識別子
				   //正しくウィンドウの作成ができたら
				   //この変数に識別子を代入する
	int width = screenWidth,	//クライアント領域の幅
	int height = screenHeight)	//クライアント領域の高さ
	//クライアント領域はウィンドウ全体から
	//外枠やメニューの部分を除いた物と今は思っておけばOK
{
	//クライアント領域のおおきさを決める
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = width;
	rect.bottom = height;

	//クライアント領域を保持するのに必要な
	//ウィンドウのおおきさを計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE/*TRUE*/);
	//正しく実行されるとrectの値が更新される

	//WS_OVERLAPPEDWINDOWはウィンドウのスタイルの一つ

	//ウィンドウのハンドル
	//ウィンドウの識別に使用する
	HWND hWnd;

	hWnd = CreateWindowEx
	(	0,						//ウィンドウ拡張スタイル
		WC_BASIC,				//作りたいウィンドウクラス
								//あらかじめ登録されたもの
		_T("タイトル"),			//ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW,	//ウィンドウのスタイル
		CW_USEDEFAULT,			//位置x座標 デフォルトの値
		CW_USEDEFAULT,			//位置y座標 デフォルトの値
		rect.right - rect.left,	//ウィンドウ全体の幅
		rect.bottom - rect.top, //ウィンドウ全体の高さ
		NULL,					//親ウィンドウのハンドル
		NULL,					//メニューのハンドル
		hInstance,				//WinMainのハンドル

		NULL);					//ウィンドウ作成データ
								//ウィンドウに好きな32ビットのデータを
								//付属させることができる
								//追加情報構造体へのポインタなど
								//を付けるのが無難な使い方か
								//今回は使わない

	//ウィンドウが正しく作れたかをチェック
	//正しく作れたらhWndにNULL以外が入るはず
	if (hWnd == NULL)
	{
		return S_FALSE;//HRESULT型　失敗を表す識別子の一つ
	}

	//ウィンドウをアクティブにして表示
	//hWndでウィンドウの識別をしていることに注目
	ShowWindow(hWnd, SW_SHOWNORMAL);

	//クライアント領域の更新
	UpdateWindow(hWnd);

	//参照渡しになっている引数に
	//ウィンドウの識別ハンドルをコピーする
	//これによって今作ったウィンドウの識別ハンドルを
	//呼び出し元のプログラムで得ることができる
	refHWnd = hWnd;

	//HRESULT型　成功を表す識別子の一つ
	return S_OK;
}

//スプライトのインスタンスを作成
//パラメータは適当で

void Reset(int light, Player *player, int x, int y, Map map);


//エントリーポイント
//プログラムの開始関数

int _stdcall WinMain
(HINSTANCE hInstance,	//プログラムを識別するハンドル
	HINSTANCE hPrevInstance,//アプリケーションの前のインスタンスハンドル
							//win32だと常にNULL
	LPSTR lpCmdLine,		//コマンドライン引数
	int nCmdShow)		//ウィンドウの表示状態
{
	//現在のゲームの状態
	enum GameMode { ZERO,START, PLAY, OVER };
	GameMode game = START;

	//光のある方角


	Player player;
	Shadow shadow;
	Map map;
	int mapNum = 9;
	map.SetMapData(mapNum);
	shadow.SetShadow(map);
	player.SetPlayer(map);

	int lightw = map.GetLightW();

	//プレイヤーの座標
	int px, py;
	//プレイヤーの向いている方向
	int pAngle = 1;
	//マップの大きさ
	const int stage1x = map.GetMapWidth(), stage1y = map.GetMapHeight();

	srand((unsigned int)time(NULL));//乱数の初期値設定
	
	if (FAILED(RegistClassEx(hInstance)))
	{
		MessageBox(NULL,
			TEXT("ウィンドウクラス設定失敗"),
			TEXT("テスト-タイトル"),
			MB_OK);
		return 0;
	}

	HWND hWnd = NULL;

	/*正しくウィンドウが作成されれば
	hWndにウィンドウの識別ハンドルが入る*/
	if (FAILED(MakeWindow(hInstance, hWnd)))
	{
		MessageBox(NULL,
			TEXT("ウィンドウ作成失敗"),
			TEXT("テスト-タイトル"),
			MB_OK);
		return 0;
	}

	//Direct3Dを管理するクラス(シングルトン)への参照を取得
	Direct3D& d3d = Direct3D::GetInstance();

	//Direct3DDeviceの作成を試みる
	if (d3d.TryCreate(hWnd))
	{
		/*MessageBox(NULL,
			TEXT("たーる"),
			TEXT("テスト-タイトル"),
			MB_OK);*/
	}

	//レンダーステートの設定  αブレンド
	d3d.SetRenderState(RENDERSTATE::RENDER_ALPHABLEND);
	
	//スプライトの問題
	Sprite  sprite;
	sprite.SetAlpha(1);							//透明度の設定
	sprite.SetSize(Pixel, Pixel);				//画像の大きさ
	sprite.SetAngle(0);							//画像の回転
	Sprite pSprite;
	pSprite.SetAlpha(1);
	pSprite.SetSize(Pixel, Pixel);
	Sprite GameMenu;
	GameMenu.SetAlpha(1);
	GameMenu.SetSize(screenWidth, screenHeight);
	GameMenu.SetPos(screenWidth / 2, screenHeight / 2);
	Sprite lSprite;
	lSprite.SetAlpha(1);
	lSprite.SetSize(Pixel * 1.5, Pixel *1.5);
	Sprite tbSprite;
	tbSprite.SetAlpha(1);
	tbSprite.SetSize(660, 160);
	tbSprite.SetPos(screenWidth / 2, screenHeight / 5 * 2 );
	

	

	

	

	

	//テクスチャのインスタンスを作成
	Texture texplayer;
	texplayer.Load(_T("Texture/sr_Player.png"));
	Texture texShadow;
	texShadow.Load(_T("Texture/sr_light.png"));
	Texture texBlockNumber1;
	texBlockNumber1.Load(_T("Texture/sr_01.png"));
	Texture texBlockNumber2;
	texBlockNumber2.Load(_T("Texture/sr_02.png"));
	Texture texBlockNumber3;
	texBlockNumber3.Load(_T("Texture/sr_03.png"));
	Texture texBlock01;
	texBlock01.Load(_T("Texture/sr_object01.png"));
	Texture texBlock02;
	texBlock02.Load(_T("Texture/sr_object02.png"));
	Texture texBlock03;
	texBlock03.Load(_T("Texture/sr_object03.png"));
	Texture texArrow;
	texArrow.Load(_T("Texture/sr_arrow.png"));	
	Texture texStair;
	texStair.Load(_T("Texture/DownStair.png"));
	Texture texTitle;
	texTitle.Load(_T("Texture/sr_title_rogo.png"));	
	Texture texTitleBack;
	texTitleBack.Load(_T("Texture/sr_title_bg.png"));
	Texture texOver;
	texOver.Load(_T("Texture/gameover.png"));
	Texture texClear;
	texClear.Load(_T("Texture/gameclear.png"));
	Texture texCursor;
	texCursor.Load(_T("Texture/sr_light_cursor.png"));
	Texture texMapBack;
	texMapBack.Load(_T("Texture/sr_game_bg.png"));	
	Texture texframe;
	texframe.Load(_T("Texture/sr_frame.png"));
	//ここで読み込んだ画像の分割処理	
	DirectInput * pDi = DirectInput::GetInstance();
	pDi->Init(hWnd);

	//メインループ
	//メッセージループ

	MSG msg = {};

	//quitメッセージが出てくるまでループを繰り返す
	//quitメッセージは上記のウィンドウプロシージャから送信
	//送信の条件などはウィンドウプロシージャを確認
	while (msg.message != WM_QUIT )
	{

		//PeekMessage
		//メッセージキューの中にメッセージがあるかを調べて
		//msgの中に書き込みtrueを返す
		//メッセージがなければfalseを返す
		//PM_REMOVEを設定しているとメッセージの書き込みのあとに
		//メッセージキューから元のメッセージを消す

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//仮想キーメッセージを文字メッセージに変換し
			//それをメッセージキューにポストする
			TranslateMessage(&msg);

			//メッセージの割り当て
			//ウィンドウプロシージャでメッセージを処理
			DispatchMessage(&msg);
		}
		else
		{
			//メッセージキューにメッセージが無かったとき
			//こちらの処理
			//ここにゲーム処理を書き込んでいく

			pDi->Update();//キー状態の更新
			if (pDi->MouseButtonJustPressed(MOUSE_BUTTON_LEFT))
				Vector2<int> vec = pDi->MousePosition();
			{
				int breakpoint = 0;
			}

			switch (game)
			{
				case ZERO:
					for (int y = 0; y < stage1y; y++)
					{
						for (int x = 0; x < stage1x; x++)
						{
							shadow.stageShadow[x+y*map.GetMapWidth()] = false;
							
						}
					}
					for (int y = 0; y < stage1y; y++)
					{
						for (int x = 0; x < stage1x; x++)
						{
							if (map.GetMapBlock(x, y) != 0 && map.GetMapBlock(x, y) != -1)
							{
								shadow.MoveShadow(x, y, map.GetMapBlock(x, y), map);
							}
						}
					}

					game = PLAY;
					break;
				case START:
					if (pDi->KeyJustPressed(DIK_RETURN))
					{
						game = ZERO;
					}
					break;
				case PLAY:
					//プレイヤーの移動に関係
					//何か方向キーを押したとき
					if (pDi->KeyJustPressed(DIK_UP) ||
						pDi->KeyJustPressed(DIK_DOWN) ||
						pDi->KeyJustPressed(DIK_LEFT) ||
						pDi->KeyJustPressed(DIK_RIGHT))
					{
						player.mPlayerY = 0;
						player.mPlayerX = 0;
						if (pDi->KeyJustPressed(DIK_UP))
						{
							pAngle = 1;
							player.mPlayerY--;
						}
						else if (pDi->KeyJustPressed(DIK_DOWN))
						{
							pAngle = 3;
							player.mPlayerY++;
						}
						else if (pDi->KeyJustPressed(DIK_LEFT))
						{
							pAngle = 4;
							player.mPlayerX--;
						}
						else if (pDi->KeyJustPressed(DIK_RIGHT))
						{
							pAngle = 2;
							player.mPlayerX++;
						}
						if (player.playerX + player.mPlayerX >= 0 && player.playerX + player.mPlayerX < map.GetMapWidth() &&
							player.playerY + player.mPlayerY >= 0 && player.playerY + player.mPlayerY < map.GetMapHeight())
						{
							player.movePlayer(map.GetMapBlock(player.playerX + player.mPlayerX, player.playerY + player.mPlayerY));
						}
					}

					px = player.playerX;
					py = player.playerY;

					//プレイヤーが前方のブロックを動かす（制限なし）
					if (pDi->KeyJustPressed(DIK_RETURN) && /*前方が床ではないかどうか*/
						map.GetMapBlock(player.playerX + player.mPlayerX, py + player.mPlayerY) > 0 &&
						map.GetMapBlock(player.playerX + player.mPlayerX, player.playerY + player.mPlayerY) < 3 &&
						map.GetMapArrow(player.playerX + player.mPlayerX, player.playerY + player.mPlayerY) == pAngle)
					{
						//押した先が床なら矢印方向を維持する

						if (map.GetMapBlock(player.playerX + player.mPlayerX * 2, player.playerY + player.mPlayerY * 2) == 0)
						{
							map.SetMapArrow(player.playerX + player.mPlayerX * 2, player.playerY + player.mPlayerY * 2, 
								map.GetMapArrow(player.playerX + player.mPlayerX, player.playerY + player.mPlayerY));
						}
						//ここで押した先に押した場所の高さ分だけプラスする
						map.SetMapBlock(player.playerX + player.mPlayerX * 2, player.playerY + player.mPlayerY * 2,
							map.GetMapBlock(player.playerX + player.mPlayerX, player.playerY + player.mPlayerY) +
							map.GetMapBlock(player.playerX + player.mPlayerX * 2, player.playerY + player.mPlayerY * 2));

						//矢印を消す
						map.SetMapArrow(player.playerX + player.mPlayerX, player.playerY + player.mPlayerY, 0);

						//押した場所を床に
						map.SetMapBlock(player.playerX + player.mPlayerX, player.playerY + player.mPlayerY, 0);


						//影の方角変更
						//ここで影の位置を変更
						for (int y = 0; y < map.GetMapHeight(); y++)
						{
							for (int x = 0; x < map.GetMapWidth(); x++)
							{
								shadow.stageShadow[x + y*map.GetMapWidth()] = false;
							}
						}
						for (int y = 0; y < map.GetMapHeight(); y++)
						{
							for (int x = 0; x < map.GetMapWidth(); x++)
							{
								if (map.GetMapBlock(x, y) != 0 && map.GetMapBlock(x, y) != -1)
								{
									shadow.MoveShadow(x, y, map.GetMapBlock(x, y), map);
								}
							}
						}
					}

					//光を回転させる
					else if (pDi->KeyJustPressed(DIK_A) || pDi->KeyJustPressed(DIK_S))
					{
						lightw = 0;
						//右回転
						if (pDi->KeyJustPressed(DIK_S))
						{
							lightw++;
						}
						else//左回転
						{
							lightw--;
						}
						shadow.MoveLight(lightw);

						//影の方角変更
						//ここで影の位置を変更
						for (int y = 0; y < map.GetMapHeight(); y++)
						{
							for (int x = 0; x < map.GetMapWidth(); x++)
							{
								shadow.stageShadow[x+y*map.GetMapWidth()] = false;
							}
						}
						for (int y = 0; y < map.GetMapHeight(); y++)
						{
							for (int x = 0; x < map.GetMapWidth(); x++)
							{
								if (map.GetMapBlock(x,y) != 0 && map.GetMapBlock(x, y) != -1)
								{
									shadow.MoveShadow(x, y, map.GetMapBlock(x, y),map);
								}
							}
						}
					}
					
					//問題発生　何かが起きた（クリアorDESU）
					if (shadow.stageShadow[player.playerX+ player.playerY*map.GetMapWidth()] == false ||
						map.GetMapBlock(px, py) == -1)
					{
						game = OVER;
					}
					
					break;
				case OVER:
					//クリア
					
					//返事がない　ただの屍のようだ
					if (shadow.stageShadow[player.playerX+player.playerY*map.GetMapWidth()] == false)
					{
						if (pDi->KeyJustPressed(DIK_C))
						{
							pAngle = 1;
							Reset(lightw, &player, px, py, map);
							shadow.SetLight(map.GetLightW());
							map.SetMapData(mapNum);
							if (pDi->KeyJustPressed(DIK_C))
							{
								game = ZERO;
							}
						}
					}
					if (pDi->KeyJustPressed(DIK_T))
					{
						pAngle = 1;
						Reset(lightw, &player, px, py, map);
						shadow.SetLight(map.GetLightW());
						map.SetMapData(mapNum);
						game = START;
					}
					break;
			}
			//まず描画 
			d3d.BeginScene();//描画開始
			//描画
			d3d.ClearScreen();
			switch (game)
			{
			case START:
				GameMenu.Draw(texTitleBack);
				
				tbSprite.Draw(texTitle);
				break;
			case OVER:
				if (map.GetMapBlock(px, py) == -1)
				{
					GameMenu.Draw(texClear);
				}
				else if (shadow.stageShadow[player.playerX + player.playerY*map.GetMapWidth()] == false)
				{
					GameMenu.Draw(texOver);
				}
				break;
			default:

				GameMenu.Draw(texMapBack);

				//マップの描画
				for (int y = 0; y < map.GetMapHeight(); y++)
				{
					for (int x = 0; x < map.GetMapWidth(); x++)
					{
						//マップの描画
						sprite.SetPos(Pixel * x - Pixel *  player.playerX + screenWidth / 2,
							Pixel * y - Pixel * player.playerY + screenHeight / 2);

						//枠の描画
						sprite.Draw(texframe);

						//ブロックの描画
						switch (map.GetMapBlock(x, y))
						{
						case 1:
							sprite.Draw(texBlock01);
							break;
						case 2:
							sprite.Draw(texBlock02);
							break;
						case 3:
							sprite.Draw(texBlock03);
							break;
						case -1:
							sprite.Draw(texStair);
							break;
						}
						//影の描画
						if (shadow.stageShadow[x+y*map.GetMapWidth()] == true && (map.GetMapBlock(x, y) == 0 || map.GetMapBlock(x, y) == -1))
						{
							shadow.DrawShadow(x, y, Pixel, player.playerX, player.playerY,
								&sprite, texShadow, screenWidth, screenHeight, map);
						}
						//矢印の描画
						if (map.GetMapArrow(x, y) > 0)
						{
							lSprite.SetPos(Pixel * x - Pixel *  player.playerX + screenWidth / 2,
								Pixel * y - Pixel * player.playerY + screenHeight / 2);
							lSprite.SetAngle(PI / 2 * (map.GetMapArrow(x, y) - 1));
							lSprite.Draw(texArrow);
						}
						//矢印がないブロックに現在の高さを描画（数字）
						else
						{
							switch (map.GetMapBlock(x, y))
							{
								case 1:
									sprite.Draw(texBlockNumber1);
									break;
								case 2:
									sprite.Draw(texBlockNumber2);
									break;
								case 3:
									sprite.Draw(texBlockNumber3);
							}
						}
					}
				}
				//現在の光の向きの描画
				lSprite.SetAngle(PI / 2 * shadow.lightWay);
				lSprite.SetPos(screenWidth - Pixel* 1.5 /2 , Pixel* 1.5 / 2 );
				lSprite.Draw(texCursor);

				//プレイヤーの描画//-----------------------------------------------------------------------
				pSprite.SetAngle(PI / 2 * (pAngle - 1));
				player.Drawplayer(screenWidth, screenHeight, &pSprite, texplayer);
				break;
			}
			
			//描画終了の合図//-------------------------------------------------------------------------
			
			
			d3d.EndScene();

			//バックバッファをフロントへ反映
			d3d.Present();
		}
	}
	return 0;
}

void Reset(int light, Player *player, int x, int y, Map map)
{
	light = 0;
	player->playerX = map.GetStartX();
	player->playerY = map.GetStartY();
	x = 0;
	y = 0;
	player->mPlayerX = 0;
	player->mPlayerY = 0;
}