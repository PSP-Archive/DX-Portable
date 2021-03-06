/****************************************************
*		DXライブラリPortable	Ver0.4.16			*
*		製作者	：夢瑞憂煉							*
*		最終更新：2009/07/13						*
*		更新履歴はファイルの最後のほうにあります。	*
****************************************************/
/****************************************************
     ---CAUTION!----
このライブラリは幾つかのライブラリを内包しています。
readme.txtに詳細がありますので、ご参照ください。
****************************************************/
/****************************************************
		備考欄										

	このライブラリについて
'DXライブラリPortable'（以降'DXP'と表記）は'DXライブラリ'（以降'本家'と表記）の不完全互換ライブラリとして製作されています。
PSPのプログラムを簡単に作れるような関数郡の提供が目的です。
現在の製作方針をいくつか挙げておきます。おおよそ上のものほど優先順位が高いです。
＊本家との互換性を可能な限り追求すること
＊可能な限り高速に動作すること				（C++ではなくCで記述しているのはこのため
＊非互換関数も可能な限り本家に似せること	（GetInputState関数は本家のGetJoypadInputState関数がモデルです
＊ユーザーモードで安全に動くこと			（プログラマのミスでPSP本体が起動不能になったらシャレにならないので…

なお、本家と完全互換の関数にはコメントをつけていません。

描画
****************************************************
****************************************************
		用語集とか									

	Swizzle（すうぃーずる）
SwizzleとはPSPのゲームプログラミングにおいて無くてはならない技術です。
プログラミングの世界ではよく参照などと訳されますが、PSPでいうSwizzleとは
GPUのキャッシュを最大限有効利用できるようにテクスチャを変換することを言います。
DXライブラリPortableでは基本的に意識しなくてもSwizzleテクスチャが使われるようになっています。
しかし、一部の関数において問題が発生します。テクスチャに直接アクセスしたい時です。
Swizzleはテクスチャ上のピクセルを並べ替えるような操作なので、それを考えずに実行すると思わぬ事態がおきます。
おそらくプログラム自体は正常に動作するでしょうが、思った結果にはならないでしょう。
で、画像を読み込む際にSwizzleを実行するかどうかのフラグを内部に設けました。
設定するにはSetCreateSwizzledGraphFlag関数を呼んでください。0以外ならSwizzleする（既定）、0ならSwizzleしないという意味です。

とか書いといて、まだグラフィックデータをいじる関数つくってないのですが（汗
なお、テクスチャをSwizzle状態と通常状態に相互に変換する関数を作りました。
SwizzleGraph関数とUnswizzleGraph関数です。速度の比較をしてにやにやしてくださいｗ

	Slice
GPUのキャッシュをできるだけ有効利用するために画像を縦に切って描画することです。DXライブラリPortableでは自動的に実行されます。
だいたい32か64バイトごとに行うと最高のパフォーマンスとなるのですが、画像によって最適な値が異なるようなので切り替えられるようにしました。
SetSliceSize関数をご利用ください。有効な引数は16,32,64,128のいずれかです。それ以外はエラーを返し、切り替えは行いません。
****************************************************/
#ifndef DXLIBP_H__
#define DXLIBP_H__
#ifdef	__cplusplus
#define	DXPDEFARG(ARG)	=(ARG)		/*C++のデフォルト引数の機能はCではエラーになるので対策*/
extern "C" {
#else
#define	DXPDEFARG(ARG)
#endif

#include <pspkernel.h>
//#include <stdio.h>

#ifndef	TRUE
#define	TRUE	(1)
#endif
#ifndef	FALSE
#define	FALSE	(0)
#endif
#ifndef	LONGLONG
#define LONGLONG u64
#endif


/*Windowsの定義をすこし流用*/
#define	BYTE	u8
#define	WORD	u16
#define	DWORD	u32

/*定数定義部*/
	/*文字セット*/
	#define DXP_CHARSET_ASCII		0x00
	#define	DXP_CHARSET_US			0x01
	#define	DXP_CHARSET_LATIN_I		0x05
	#define	DXP_CHARSET_LATIN_II	0x13
	#define	DXP_CHARSET_RUSSIAN		0x0b
	#define DXP_CHARSET_SHIFTJIS	0x0d
	#define	DXP_CHARSET_GBK			0x0e
	#define	DXP_CHARSET_KOREAN		0x0f
	#define	DXP_CHARSET_BIG5		0x10
	#define	DXP_CHARSET_CYRILLIC	0x12
	#define	DXP_CHARSET_UTF8		0xff
	#define	DXP_CHARSET_DEFAULT		DXP_CHARSET_SHIFTJIS
	/*ファイルの読み込み*/
	#define DXP_FILEREAD_DOS	0x00000000
	#define DXP_FILEREAD_UNIX	0x00000001
	/*描画*/
	#define DXP_SCREEN_BACK		0xfffffffe

	/*グラフィックのフォーマット定義*/
	#define DXP_FMT_5650		(0)	/* テクスチャ、パレット、描画先*/
	#define DXP_FMT_5551		(1)	/* テクスチャ、パレット、描画先*/
	#define DXP_FMT_4444		(2)	/* テクスチャ、パレット、描画先*/
	#define DXP_FMT_8888		(3)	/* テクスチャ、パレット、描画先*/
	#define DXP_FMT_T4			(4) /* テクスチャのみ */
	#define DXP_FMT_T8			(5) /* テクスチャのみ */
	#define DXP_FMT_DXT1		(8) /* テクスチャのみ */
	#define DXP_FMT_DXT3		(9) /* テクスチャのみ */
	#define DXP_FMT_DXT5		(10)/* テクスチャのみ */

	/*描画モード定義*/
	#define DX_DRAWMODE_NEAREST	0
	#define DX_DRAWMODE_BILINEAR	1

	 /*描画ブレンドモード定義*/
	#define DX_BLENDMODE_NOBLEND						(0)				/* ノーブレンド*/
	#define DX_BLENDMODE_ALPHA							(1)				/* αブレンド*/
	#define DX_BLENDMODE_ADD							(2)				/* 加算ブレンド*/
	#define DX_BLENDMODE_SUB							(3)				/* 減算ブレンド*/
	#define DX_BLENDMODE_MUL							(4)				/* 乗算ブレンド*/
	#define DX_BLENDMODE_DESTCOLOR						(8)				/* カラーは更新されない*/
	#define DX_BLENDMODE_INVDESTCOLOR					(9)				/* 描画先の色の反転値を掛ける*/
	#define DX_BLENDMODE_INVSRC						(10)			/* 描画元の色を反転する*/
	/*#define DX_BLENDMODE_MULA						(11)		*/
		/* アルファチャンネル考慮付き乗算ブレンドはPSPの制限で実装できませんでした＞＜。*/

	/*入力*/
		/*ボタン入力*/
	#define DXP_INPUT_UP		(0x00000010)
	#define DXP_INPUT_RIGHT		(0x00000020)
	#define DXP_INPUT_DOWN		(0x00000040)
	#define DXP_INPUT_LEFT		(0x00000080)
	#define DXP_INPUT_TRIANGLE	(0x00001000)
	#define DXP_INPUT_CIRCLE	(0x00002000)
	#define DXP_INPUT_CROSS		(0x00004000)
	#define DXP_INPUT_SQUARE	(0x00008000)
	#define DXP_INPUT_LTRIGGER	(0x00000100)
	#define DXP_INPUT_RTRIGGER	(0x00000200)
	#define DXP_INPUT_START		(0x00000008)
	#define DXP_INPUT_SELECT	(0x00000001)
		/*オンスクリーンキーボード入力*/
	#define	DXP_OSK_ALL			(0x00000000)	//全て
	#define	DXP_OSK_DIGIT		(0x00000001)	//半角数字
	#define	DXP_OSK_SYMBOL		(0x00000002)	//半角記号
	#define	DXP_OSK_SMALL		(0x00000004)	//半角小文字
	#define	DXP_OSK_LARGE		(0x00000008)	//半角大文字
	#define	DXP_OSK_DIGIT2		(0x00000100)	//全角数字
	#define	DXP_OSK_SYMBOL2		(0x00000200)	//全角記号
	#define	DXP_OSK_SMALL2		(0x00000400)	//全角小文字
	#define	DXP_OSK_LARGE2		(0x00000800)	//全角大文字
	#define	DXP_OSK_HIRAGANA	(0x00001000)	//ひらがな
	#define	DXP_OSK_KATAKANA_H	(0x00002000)	//半角カタカナ
	#define	DXP_OSK_KATAKANA	(0x00004000)	//全角カタカナ
	#define	DXP_OSK_KANJI		(0x00008000)	//漢字
	#define	DXP_OSK_URL			(0x00080000)	//URL入力支援

	#define PSPCTRLMODE_MSWP		0x00000001
	#define PSPCTRLMODE_KB			0x00000002
	#define PSPANALOGMODE_CKEY		0x00000010
	#define PSPANALOGMODE_NOCKEY	0x00000020

	/*音楽再生*/
	#define DX_PLAYTYPE_NORMAL							(0)												// ノーマル再生
	#define DX_PLAYTYPE_BACK				  			(1)							// バックグラウンド再生
	#define DX_PLAYTYPE_LOOP							(3)	// ループ再生

	#define DXP_FONT_ALIGN_LEFT		0x0000		//左寄せ
	#define DXP_FONT_ALIGN_CENTER	0x0200		//中央
	#define DXP_FONT_ALIGN_RIGHT	0x0400		//右寄せ
	#define DXP_FONT_WIDTH_FIX		0x0800		//等幅指定用
	#define DXP_FONT_ALIGN_DEFAULT	(DXP_FONT_ALIGN_LEFT)
	#define DXP_FONT_COLOR_NONE		0x00000000	//透明
	#define DXP_FONT_COLOR_BLACK	0xff000000
	#define DXP_FONT_COLOR_RED 		0xff0000ff
	#define DXP_FONT_COLOR_GREEN	0xff00ff00
	#define DXP_FONT_COLOR_BLUE		0xffff0000
	#define DXP_FONT_COLOR_WHITE	0xffffffff
	#define DXP_FONT_COLOR_LITEGRAY	0xffbfbfbf
	#define DXP_FONT_COLOR_GRAY		0xff7f7f7f
	#define DXP_FONT_COLOR_DARKGRAY	0xff3f3f3f
	#define DXP_FONT_DEFAULT_SIZE	16			//1.0fの時のドットサイズ

/*構造体定義部*/
	typedef struct tagVECTOR
	{
		float	x, y, z ;
	}VECTOR, *LPVECTOR ;

	typedef struct tagVERTEX_3D
	{
		VECTOR					pos ;
		unsigned char			b, g, r, a ;
		float					u, v ;
	}VERTEX_3D,*LPVERTEX_3D;
/*関数定義部*/
	/*必須関数*/
		int DxLib_IsInit();
		int DxLib_Init();
		int DxLib_End();
		int ProcessMessage();
	/*汎用関数*/
		/*設定系*/
		/*取得系*/
		/*機能系*/
		int GetNowCount();
		u64 GetNowHiPerformanceCount();
		int	SRand(int RandSeed);
		int	GetRand(int RandMax);
		int Sleep(int time);							//実はWindowsAPIなんですがｗ
		int WaitTimer(int time);
		void AppLogAdd(const char *Format,...);
		int sjis2unicode(u16 sjis,u16 *punicode);		//sjisの文字をUnicodeにします
		int unicode2sjis(u16 unicode,u16 *psjis);		//Unicodeの文字をsjisにします
		int unicodestr2sjisstr(const u16 *unis,char *sjiss);//第一引数の文字列を第二引数のバッファに格納します。
		int sjisstr2unicodestr(const char *sjiss,u16 *unis);//第一引数の文字列を第二引数のバッファに格納します。
#define AppLogAdd2(FMT,...)	AppLogAdd("%s,%s,%d,%s\t"FMT,__TIME__,__FILE__,__LINE__,__func__,##__VA_ARGS__);
	/*入力関連関数*/
		int InitInput();							/*初期化。通常は呼ばなくてもいい*/
		int RenewInput();							/*更新。ProcessMessageの中で呼ばれるので通常は呼ばなくてもいい*/
		int GetInputState();						/*ボタン入力の情報を得る*/
		int GetAnalogInput(int *XBuf,int *YBuf);	/*アナログパッドの入力を得る。範囲は±1000。遊び判定が必要です。*/
		int GetHitKeyStateAll( char *KeyStateBuf );	/*全ての入力バッファに0を格納し、かつ戻り値として0を返します。*/
		int GetJoypadInputState( int InputType );	/*PSPの入力をJOYPAD入力に見立てて返します。DXライブラリに互換を取るための関数です。*/
		int GetTextOSK(char *buf,int buflen,int inputmode,const char *title DXPDEFARG(NULL),const char *init DXPDEFARG(NULL));
													/*オンスクリーンキーボードを使います。	*/
													/*buf		：取得した文字列の格納先	*/
													/*buflen	：最大文字数				*/
													/*inputmode	：DXP_OSKMODEのどれか		*/
													/*title		：タイトル文字列			*/
													/*init		：初期文字列				*/
		int	SetPspCtrlMode(int Mode);
		int	SetPspAnalogMode(int Mode);

	/*描画関連関数*/
		/*設定系*/
		int SetDisplayFormat(int format);				/*画面のフォーマットを変更します。DXP_FMT_5650を指定するとマスク機能が使えなくなるので注意してください。*/
		int SetDrawScreen(int ghandle);
		int SetDrawMode(int mode);
		int SetDrawBlendMode(int BlendMode,int Param);
		int SetDrawBright(int red,int green,int blue);
		int SetCreateSwizzledGraphFlag(int Flag);		/*画像の作成、読み込み時（MakeGraph除く）にSwizzle状態にするか指定*/
		int SetSliceSize(int size);						/*描画時にsliceが可能な場合、何バイト境界で行うか指定*/
		int SetTransColor( int Red , int Green , int Blue );
		int SetDrawArea(int x1,int y1,int x2,int y2);
		/*取得系*/
		int GetDisplayFormat();				/*画面のフォーマットを取得*/
		int GetColor(int red,int green,int blue);
		int GraphSize2DataSize(int width,int height,int Format);				/*縦横の大きさとフォーマットからデータサイズを計算する。*/
		int AppLogAdd_GraphicData(int gh);										/*グラフィックハンドルの情報を書き出す。デバッグ専用*/
		int GetGraphSize(int gh,int *px,int *py);
		/*読み込み系*/
		int MakeGraph(int x,int y,int format DXPDEFARG(DXP_FMT_8888));		/*最後の引数に画像フォーマットを記述する必要があります*/
		int LoadRAWData(const char *FileName,int SizeX,int SizeY,int Format);/*RAWファイルを読み込むのに使います*/
		int LoadDivGraph( const char *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf);
		int DerivationGraph( int SrcX, int SrcY,int Width, int Height, int src );
		int LoadGraph(const char *FileName);
		int DeleteGraph(int GrHandle);
		int ConvertGraphFormat(int gh,int psm);/*グラフィックのフォーマットを変更する。メモリの節約と高速化にどうぞ。DX_SCREEN_BACKには適用できません*/
		/*描画系*/
		/*
				〜ほとんどの描画系関数に対する注意〜
			PSPのハードウェアの制約上、内部的に指定される座標が±4000付近を超えるとおかしな描画になります。
			特に、DrawRotaGraphでは拡大率の上げすぎに注意してください。うまいこと回避する方法を現在考え中です。
		*/
		int	ScreenFlipMode(int Mode);
		int ScreenFlip();
		int ScreenCopy();
		int ClearDrawScreen();
		int	DrawPixel( int x, int y, int Color);
		int	DrawLine( int x1, int y1, int x2, int y2, int Color);
		int DrawTriangle(int x1,int y1,int x2,int y2,int x3,int y3,int color,int fill);
		int DrawBox(int x1,int y1,int x2,int y2,int color,int fillflag);
		int	DrawCircle( int x, int y, int r, int Color,int fill);
		int	DrawGraph(int x,int y,int gh,int trans);
		int DrawGraphCentre(int x,int y,int gh,int trans);/*描画座標が画像の中心となります*/
		int DrawExtendGraph(int x1,int y1,int x2,int y2,int gh,int trans);
		int	DrawModiGraph(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,int gh,int trans);
		int	DrawTurnGraph(int x,int y,int gh,int trans);
		int	DrawRotaGraph(int x,int y,float ExtRate,float Angle,int gh,int trans,int turn DXPDEFARG(0));
		int DrawRotaGraph2(int x,int y,int cx,int cy,float ExtRate,float Angle,int gh,int trans,int turn DXPDEFARG(0));

		int	DrawModiGraphF(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4,int gh,int trans);
		int	DrawRotaGraphF(float x,float y,float ExtRate,float Angle,int gh,int trans,int turn DXPDEFARG(0));
		int	DrawRotaGraph2F(float x,float y,float cx,float cy,float ExtRate,float Angle,int gh,int trans,int turn DXPDEFARG(0));
		int DrawPolygon3D(VERTEX_3D *Vertex,int PolygonNum,int GrHandle,int TransFlag);//テスト中です…
		/*引数を本家に合わせた（double型を使っている）バージョン*/
		int	DrawRotaGraphCompatible(int x,int y,double ExtRate,double Angle,int gh,int trans,int turn DXPDEFARG(0));
		int DrawRotaGraph2Compatible(int x,int y,int cx,int cy,double ExtRate,double Angle,int gh,int trans,int turn DXPDEFARG(0));
		int	DrawRotaGraphFCompatible(float x,float y,double ExtRate,double Angle,int gh,int trans,int turn DXPDEFARG(0));
		int	DrawRotaGraph2FCompatible(float x,float y,float cx,float cy,double ExtRate,double Angle,int gh,int trans,int turn DXPDEFARG(0));

		/*処理速度アップ用関数
		SetTextureで指定されたグラフィックを用いて描画します
		テクスチャの検索と切り替えの処理を省略できるので、弾幕ゲー等の同じグラフィックを大量に使うアプリに役立ちます
		Boost系以外の描画関数が呼ばれた時点で再度SetTextureを呼ばない限り描画できなくなります
		*/
		int SetTexture(int handle,int TransFlag);
		void WaitDrawProcess();/*今までに発行された全ての描画コマンドの実行が終わるまで待ちます*/
		int DrawGraphBoost(int x,int y);
		int DrawGraphCentreBoost(int x,int y);/*描画座標が画像の中心となります*/
		int DrawExtendGraphBoost(int x1,int y1,int x2,int y2);
		int	DrawRotaGraphFBoost(float x,float y,float ExtRate,float Angle,int turn DXPDEFARG(0));
		/*その他*/
		int SwizzleGraph(int gh);	/*指定されたグラフィックをSwizzleする。ただし、メインメモリの空き容量が足りない（グラフィックの実サイズより少ない）と失敗します。*/
		int UnswizzleGraph(int gh);	/*指定されたグラフィックをUnswizzleする。ただし、メインメモリの空き容量が足りない（グラフィックの実サイズより少ない）と失敗します。*/
		int MoveGraphToDDR(int gh);	/*指定されたグラフィックがVRAM上にある場合はメインメモリに移動*/
		int MoveGraphToVRAM(int gh);/*指定されたグラフィックがメインメモリにある場合はVRAM上に移動*/
		void WaitGPUSync();			/*GPUに発行された命令が全て処理されるまで待つ*/
	/*文字列描画関連関数*/
		int InitString();			/*はじめてDrawString系関数が呼ばれたときに呼び出されます。あらかじめ呼び出しておくのも手です。*/
		int EndString();			/*フォントデータを全てメモリ上から開放します。（デフォルトフォント含む）*/
		int DrawString( int x, int y, const char *String, int Color, int EdgeColor DXPDEFARG(0));/*まだ正常に動作しません！*/
		int DrawFormatString(int x,int y,int color,const char *String,...);
		int	DrawFormatStringToHandle( int x, int y, int Color, int FontHandle, const char *FormatString, ... );
		int	CreateFontToHandle( const char *FontName, int Size, int Thick, int FontType , int CharSet , int EdgeSize , int Italic  , int DataIndex , int ID );
		int SetFontSize( int FontSize );
		int SetFontSizeF( float FontSize );
		int SetFontBackgroundColor(int Color);
		int SetFontAlignment(int Position,int Width);/*文字列の並び方を指定。DXP_FONT_ALIGN*/

	/*音楽関連関数*/
		/*設定系*/
		int	SetCreateSoundDataType( int SoundDataType );
		int	ChangeVolumeSoundMem( int VolumePal, int SoundHandle );
		int	SetLoopPosSoundMem( int LoopTime, int SoundHandle );
		/*取得系*/
		int	CheckSoundMem( int handle );
		/*読み込み系*/
		int LoadSoundMem(const char* filename);
		int LoadStreamSound(const char *filename,int SetPcmLen DXPDEFARG(-1),int* AnsPcmLen DXPDEFARG(NULL));
		int	DeleteSoundMem( int SoundHandle, int LogOutFlag );
		/*再生系*/
		int PlaySoundMem(int SoundHandle,int PlayType/*,int TopPositionFlag DXPDEFARG(1)*/);
		int StopSoundMem(int handle);
		int PlayStreamSound(int SoundHandle,int PlayType/*,int TopPositionFlag DXPDEFARG(1)*/);
		int getRefCount(int sh);
		//20090415
		int PlaySoundFile(const char *FileName , int PlayType ,int SetPcmLen DXPDEFARG(-1),int* AnsPcmLen DXPDEFARG(NULL));
		int CheckSoundFile();
		int StopSoundFile();
		//20090425
		int	SetTopPosSoundMem(int handle );

	/*ファイル読み込み関数*/
		int FileRead_SetMode(int Mode);		/*Windowsで作ったテキストの改行コード対策*/
		int FileRead_open(const char *FileName);
		int FileRead_close(int FileHandle);
		int FileRead_size(const char *FileName);
		int FileRead_tell(int FileHandle);
		int FileRead_seek(int FileHandle,int Offset,int Origin);
		int FileRead_write(void * Buffer,int Size,int Num,int FileHandle);
		int FileRead_stdread(void * Buffer,int Size,int Num,int FileHandle);
		int FileRead_read(void * Buffer,int Size,int FileHandle);
		int FileRead_eof(int FileHandle);
		int FileRead_gets(char *Buffer,int Num,int FileHandle);
		int FileRead_getc(int FileHandle);
		int FileRead_scanf(int FileHandle,const char* Format,...);
		int FileRead_idle_chk( int FileHandle );/*現在常に1を返します*/
	/*簡易文字列描画*/
		void printfDx(const char *format,...);	/*一列あたり半角80文字で22列あります。内部バッファは2048文字で、それ以上は描画されません。それ以外は本家ライブラリと同じです。*/
		void clsDx();
		void DrawString_Shinonome(int x,int y,const char *str,int color);/*printfDxで使っている全角描画ライブラリを呼び出せます。*/
	/*マイナー系*/
		int SaveDrawScreen( int x1, int y1, int x2, int y2, char *FileName );//現在動作しません
	/*デバッグ用*/
		//int GetCpuUsage();//製作中

/*DXライブラリからコピペ*/
	//20090405
	//DXライブラリ本家よりコピー
	#define PAD_INPUT_DOWN								(0x00000001)	// ↓チェックマスク
	#define PAD_INPUT_LEFT								(0x00000002)	// ←チェックマスク
	#define PAD_INPUT_RIGHT								(0x00000004)	// →チェックマスク
	#define PAD_INPUT_UP								(0x00000008)	// ↑チェックマスク
	#define PAD_INPUT_A									(0x00000010)	// Ａボタンチェックマスク
	#define PAD_INPUT_B									(0x00000020)	// Ｂボタンチェックマスク
	#define PAD_INPUT_C									(0x00000040)	// Ｃボタンチェックマスク
	#define PAD_INPUT_X									(0x00000080)	// Ｘボタンチェックマスク
	#define PAD_INPUT_Y									(0x00000100)	// Ｙボタンチェックマスク
	#define PAD_INPUT_Z									(0x00000200)	// Ｚボタンチェックマスク
	#define PAD_INPUT_L									(0x00000400)	// Ｌボタンチェックマスク
	#define PAD_INPUT_R									(0x00000800)	// Ｒボタンチェックマスク
	#define PAD_INPUT_START								(0x00001000)	// ＳＴＡＲＴボタンチェックマスク
	#define PAD_INPUT_M									(0x00002000)	// Ｍボタンチェックマスク
	//20090405
	#define KEY_INPUT_ESCAPE	0x0E
	#define KEY_INPUT_LEFT		0xCB
	#define KEY_INPUT_UP		0xC8
	#define KEY_INPUT_RIGHT		0xCD
	#define KEY_INPUT_DOWN		0xD0
	#define KEY_INPUT_LSHIFT	0x2A
	#define KEY_INPUT_LCONTROL	0x1D
	#define KEY_INPUT_Z			0x2C
	#define KEY_INPUT_X			0x2D
	#define	KEY_INPUT_A			0x1e
	#define	KEY_INPUT_S			0x1f
	#define	KEY_INPUT_Q			0x10
	#define	KEY_INPUT_W			0x11
	#define	KEY_INPUT_SPACE		0x39
	//20090405
	// パッド入力取得パラメータ
	#define DX_INPUT_KEY_PAD1							(0x1001)		// キー入力とパッド１入力
	#define DX_INPUT_PAD1								(0x0001)		// パッド１入力
	#define DX_INPUT_PAD2								(0x0002)		// パッド２入力
	#define DX_INPUT_PAD3								(0x0003)		// パッド３入力
	#define DX_INPUT_PAD4								(0x0004)		// パッド４入力
	#define DX_INPUT_PAD5								(0x0005)		// パッド５入力
	#define DX_INPUT_PAD6								(0x0006)		// パッド６入力
	#define DX_INPUT_PAD7								(0x0007)		// パッド７入力
	#define DX_INPUT_PAD8								(0x0008)		// パッド８入力
	#define DX_INPUT_KEY								(0x1000)		// キー入力

// ストリームデータ制御用関数ポインタ構造体
typedef struct
{
	long					(*Tell)( void *StreamDataPoint ) ;
	int						(*Seek)( void *StreamDataPoint, long SeekPoint, int SeekType ) ;
	size_t					(*Read)( void *Buffer, size_t BlockSize, size_t DataNum, void *StreamDataPoint ) ;
	size_t					(*Write)( void *Buffer, size_t BlockSize, size_t DataNum, void *StreamDataPoint ) ;
	int						(*Eof)( void *StreamDataPoint ) ;
	int						(*IdleCheck)( void *StreamDataPoint ) ;
	int						(*Close)( void *StreamDataPoint ) ;
} STREAMDATASHRED, *LPSTREAMDATASHRED ;

// ストリームデータ制御用データ構造体
typedef struct
{
	STREAMDATASHRED			ReadShred ;
	void					*DataPoint ;
} STREAMDATA ;
// ストリームデータ制御のシークタイプ定義
#define STREAM_SEEKTYPE_SET							(SEEK_SET)
#define STREAM_SEEKTYPE_END							(SEEK_END)
#define STREAM_SEEKTYPE_CUR							(SEEK_CUR)

#define STTELL( st )								((st)->ReadShred.Tell( (st)->DataPoint ))
#define STSEEK( st, pos, type )						((st)->ReadShred.Seek( (st)->DataPoint, (pos), (type) ))
#define STREAD( buf, length, num, st )				((st)->ReadShred.Read( (buf), (length), (num), (st)->DataPoint ))
#define STWRITE( buf, length, num, st )				((st)->ReadShred.Write( (buf), (length), (num), (st)->DataPoint ))
#define STEOF( st )									((st)->ReadShred.Eof( (st)->DataPoint ))
#define STCLOSE( st )								((st)->ReadShred.Close( (st)->DataPoint ))

int SetupSTREAMDATA(const char *FileName,STREAMDATA *DataPtr);	/*ファイル名からSTREAMDATA構造体をセットアップ。*/



#ifdef	__cplusplus
};
#endif

#endif

/*
更新履歴

Ver0.0.0		公開開始
Ver0.0.1		恥ずかしいバグを修正
Ver0.0.2		高速化＆バグ修正
Ver0.1.0		Sliceの導入、DeleteGraph関数を作り忘れていたので追加、TransFlagを使えるようにした、ConvertGraphFormat関数を追加、その他バグ修正
Ver0.1.1		DrawRotaGraphの計算に恥ずかしいミスがあったので修正。全ての画像描画関数でSliceを利用するように変更
Ver0.2.0		MP3ファイルをPlayMusicで再生できるようにしました。WAVは再生できないので注意です
Ver0.2.1		printfDx系関数を全面書き直ししました。DrawString_Shinonome関数を追加しました。
Ver0.3.0		GetTextOSK関数を追加しました。
Ver0.3.1		ファイルの取り扱いに関する致命的なバグを修正
Ver0.3.2		libjpegとPSP内部のJpegデコーダのどちらを使うかをDXPコンパイル時に選択可能にした。
Ver0.4.0		intraFontを内包し、PSP本体に組み込まれているフォントを利用可能にした。音楽再生機能の再構築をした。
Ver0.4.1		DrawExtendGraphで表示結果が反転する時とDrawTurnGraph使用時で描画結果がおかしくなる問題を修正。（前記に該当する場合は内部的にDrawModiGraphを使うため若干の速度低下が発生します。
Ver0.4.2		GetAnalogInputが不正な値を返す問題を修正。
Ver0.4.3		LoadDivGraph関数とDerivationGraph関数を実装。
Ver0.4.4		内部構造の一新とSetDrawScreen関数の追加。SetDrawScreenに渡すグラフィックハンドルはVRAM上にある必要があります。
Ver0.4.5		音楽再生機能のバグ修正（シューティング大好き氏によるソースコード改修）、描画ルーチンの高速化などです。
Ver0.4.6		しょうもないミスタイプで描画結果がおかしくなっていたのを修正
Ver0.4.7
Ver0.4.8		バグ修正
Ver0.4.9		ブレンドモードの仕様を本家に合わせた。
Ver0.4.10		加算合成が正常に働かないバグを修正
Ver0.4.11		パレット形式のPNG画像はパレットを使って描画するように修正
Ver0.4.12		MP3の再生を安定化、文字列描画のバグ修正、その他いろいろ
Ver0.4.14		描画機能を更に高速化
Ver0.4.16		DrawRotaGraphF内部の計算式最適化、命名規則にそぐわない関数名の変更、Boost系関数のエラーチェック、HOMEボタンでゲームを終了する際に即強制終了する動作だったのをProcessMessageで-1を返すように変更、その他バグ修正がいっぱい！
*/

