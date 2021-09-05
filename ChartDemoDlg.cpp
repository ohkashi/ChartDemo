// ChartDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDemo.h"
#include "ChartDemoDlg.h"

#include "ChartLineSerie.h"
#include "ChartPointsSerie.h"
#include "ChartSurfaceSerie.h"
#include "ChartCandlestickSerie.h"
#include "ChartGrid.h"

#include "SeriesPropDlg.h"
#include "LinePropDialog.h"
#include "SurfacePropDialog.h"
#include "PointsPropDialog.h"

#include "ChartBarSerie.h"
#include "ChartLabel.h"

#include "ChartAxisLabel.h"
#include "ChartStandardAxis.h"
#include "ChartDateTimeAxis.h"

#include "ChartCrossHairCursor.h"
#include "ChartDragLineCursor.h"

#include <math.h>
#include <stdlib.h>
#include <sstream>
#include <iomanip>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartDemoDlg dialog

CChartDemoDlg::CChartDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChartDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChartDemoDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChartDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChartDemoDlg)
	DDX_Control(pDX, IDC_TITLE_EDIT, m_TitlesEdit);
	DDX_Control(pDX, IDC_SERIES_LIST, m_SeriesList);
	DDX_Control(pDX, IDC_AXISMINVAL_EDIT, m_AxisMinValEdit);
	DDX_Control(pDX, IDC_AXISMAXVAL_EDIT, m_AxisMaxValEdit);
	DDX_Control(pDX, IDC_LEGENDVIS_CHECK, m_LegendVisBtn);
	DDX_Control(pDX, IDC_BKGND_COLBTN, m_BackgndColSel);
	DDX_Control(pDX, IDC_CHARTCTRL, m_ChartCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChartDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CChartDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADDSERIES, OnAddseries)
	ON_BN_CLICKED(IDC_LEGENDVIS_CHECK, OnLegendVisible)
	ON_BN_CLICKED(IDC_BOTTOMAXIS_RADIO, OnBottomAxisRadio)
	ON_BN_CLICKED(IDC_LEFTAXIS_RADIO, OnLeftAxisRadio)
	ON_BN_CLICKED(IDC_RIGHTAXIS_RADIO, OnRightAxisRadio)
	ON_BN_CLICKED(IDC_TOPAXIS_RADIO, OnTopAxisRadio)
	ON_BN_CLICKED(IDC_AXISAUTOMATIC_CHECK, OnAxisAutomaticCheck)
	ON_BN_CLICKED(IDC_AXISGRIDVIS_CHECK, OnAxisGridVisCheck)
	ON_BN_CLICKED(IDC_AXISVISIBLE_CHECK, OnAxisVisibleCheck)
	ON_BN_CLICKED(IDC_AXISSCROLLBAR_CHECK, OnAxisScrollBarCheck)	
	ON_MESSAGE(CPN_SELENDOK,    OnChangeBckCol)
	ON_EN_KILLFOCUS(IDC_AXISMAXVAL_EDIT, OnChangeAxisMax)
	ON_EN_KILLFOCUS(IDC_AXISMINVAL_EDIT, OnChangeAxisMin)
	ON_BN_CLICKED(IDC_AXISINVERTED_CHECK, OnAxisInvertedCheck)
	ON_EN_KILLFOCUS(IDC_AXISLABEL_EDIT, OnChangeAxisLabel)
	ON_BN_CLICKED(IDC_DELETESERIES, OnDeleteSeries)
	ON_EN_KILLFOCUS(IDC_TITLE_EDIT, OnChangeTitle)
	ON_BN_CLICKED(IDC_PAN_CHECK, OnPanCheck)
	ON_BN_CLICKED(IDC_ZOOM_CHECK, OnZoomCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartDemoDlg message handlers

BOOL CChartDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	((CButton*)GetDlgItem(IDC_RIGHTAXIS_RADIO))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_PAN_CHECK))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_ZOOM_CHECK))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_LEGENDVIS_CHECK))->SetCheck(1);

	m_ChartCtrl.SetBackColor(RGB(38, 38, 38));
	m_ChartCtrl.SetBorderColor(RGB(92, 90, 88));

	//CChartStandardAxis* pBottomAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	//pBottomAxis->SetTextColor(RGB(210, 210, 210));
	//pBottomAxis->SetMinMax(0, 10);
	CChartStandardAxis* pRightAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::RightAxis);
	pRightAxis->SetTextColor(RGB(210, 210, 210));
	pRightAxis->SetMinMax(0, 10);
	/*CChartStandardAxis* pTopAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::TopAxis);
	pTopAxis->SetTextColor(RGB(210, 210, 210));
	pTopAxis->SetMinMax(0, 10);
	CChartStandardAxis* pRightAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::RightAxis);
	pRightAxis->SetTextColor(RGB(210, 210, 210));
	pRightAxis->SetMinMax(0, 10);*/

	const double dates[] = { 1546300800,1546387200,1546473600,1546560000,1546819200,1546905600,1546992000,1547078400,1547164800,1547424000,1547510400,1547596800,1547683200,1547769600,1547942400,1548028800,1548115200,1548201600,1548288000,1548374400,1548633600,1548720000,1548806400,1548892800,1548979200,1549238400,1549324800,1549411200,1549497600,1549584000,1549843200,1549929600,1550016000,1550102400,1550188800,1550361600,1550448000,1550534400,1550620800,1550707200,1550793600,1551052800,1551139200,1551225600,1551312000,1551398400,1551657600,1551744000,1551830400,1551916800,1552003200,1552262400,1552348800,1552435200,1552521600,1552608000,1552867200,1552953600,1553040000,1553126400,1553212800,1553472000,1553558400,1553644800,1553731200,1553817600,1554076800,1554163200,1554249600,1554336000,1554422400,1554681600,1554768000,1554854400,1554940800,1555027200,1555286400,1555372800,1555459200,1555545600,1555632000,1555891200,1555977600,1556064000,1556150400,1556236800,1556496000,1556582400,1556668800,1556755200,1556841600,1557100800,1557187200,1557273600,1557360000,1557446400,1557705600,1557792000,1557878400,1557964800,1558051200,1558310400,1558396800,1558483200,1558569600,1558656000,1558828800,1558915200,1559001600,1559088000,1559174400,1559260800,1559520000,1559606400,1559692800,1559779200,1559865600,1560124800,1560211200,1560297600,1560384000,1560470400,1560729600,1560816000,1560902400,1560988800,1561075200,1561334400,1561420800,1561507200,1561593600,1561680000,1561939200,1562025600,1562112000,1562198400,1562284800,1562544000,1562630400,1562716800,1562803200,1562889600,1563148800,1563235200,1563321600,1563408000,1563494400,1563753600,1563840000,1563926400,1564012800,1564099200,1564358400,1564444800,1564531200,1564617600,1564704000,1564963200,1565049600,1565136000,1565222400,1565308800,1565568000,1565654400,1565740800,1565827200,1565913600,1566172800,1566259200,1566345600,1566432000,1566518400,1566777600,1566864000,1566950400,1567036800,1567123200,1567296000,1567382400,1567468800,1567555200,1567641600,1567728000,1567987200,1568073600,1568160000,1568246400,1568332800,1568592000,1568678400,1568764800,1568851200,1568937600,1569196800,1569283200,1569369600,1569456000,1569542400,1569801600,1569888000,1569974400,1570060800,1570147200,1570406400,1570492800,1570579200,1570665600,1570752000,1571011200,1571097600,1571184000,1571270400,1571356800,1571616000,1571702400,1571788800,1571875200,1571961600 };
	const double opens[] = { 1284.7,1319.9,1318.7,1328,1317.6,1321.6,1314.3,1325,1319.3,1323.1,1324.7,1321.3,1323.5,1322,1281.3,1281.95,1311.1,1315,1314,1313.1,1331.9,1334.2,1341.3,1350.6,1349.8,1346.4,1343.4,1344.9,1335.6,1337.9,1342.5,1337,1338.6,1337,1340.4,1324.65,1324.35,1349.5,1371.3,1367.9,1351.3,1357.8,1356.1,1356,1347.6,1339.1,1320.6,1311.8,1314,1312.4,1312.3,1323.5,1319.1,1327.2,1332.1,1320.3,1323.1,1328,1330.9,1338,1333,1335.3,1345.2,1341.1,1332.5,1314,1314.4,1310.7,1314,1313.1,1315,1313.7,1320,1326.5,1329.2,1314.2,1312.3,1309.5,1297.4,1293.7,1277.9,1295.8,1295.2,1290.3,1294.2,1298,1306.4,1299.8,1302.3,1297,1289.6,1302,1300.7,1303.5,1300.5,1303.2,1306,1318.7,1315,1314.5,1304.1,1294.7,1293.7,1291.2,1290.2,1300.4,1284.2,1284.25,1301.8,1295.9,1296.2,1304.4,1323.1,1340.9,1341,1348,1351.4,1351.4,1343.5,1342.3,1349,1357.6,1357.1,1354.7,1361.4,1375.2,1403.5,1414.7,1433.2,1438,1423.6,1424.4,1418,1399.5,1435.5,1421.25,1434.1,1412.4,1409.8,1412.2,1433.4,1418.4,1429,1428.8,1420.6,1441,1460.4,1441.7,1438.4,1431,1439.3,1427.4,1431.9,1439.5,1443.7,1425.6,1457.5,1451.2,1481.1,1486.7,1512.1,1515.9,1509.2,1522.3,1513,1526.6,1533.9,1523,1506.3,1518.4,1512.4,1508.8,1545.4,1537.3,1551.8,1549.4,1536.9,1535.25,1537.95,1535.2,1556,1561.4,1525.6,1516.4,1507,1493.9,1504.9,1506.5,1513.1,1506.5,1509.7,1502,1506.8,1521.5,1529.8,1539.8,1510.9,1511.8,1501.7,1478,1485.4,1505.6,1511.6,1518.6,1498.7,1510.9,1510.8,1498.3,1492,1497.7,1484.8,1494.2,1495.6,1495.6,1487.5,1491.1,1495.1,1506.4 };
	const double highs[] = { 1284.75,1320.6,1327,1330.8,1326.8,1321.6,1326,1328,1325.8,1327.1,1326,1326,1323.5,1322.1,1282.7,1282.95,1315.8,1316.3,1314,1333.2,1334.7,1341.7,1353.2,1354.6,1352.2,1346.4,1345.7,1344.9,1340.7,1344.2,1342.7,1342.1,1345.2,1342,1350,1324.95,1330.75,1369.6,1374.3,1368.4,1359.8,1359,1357,1356,1353.4,1340.6,1322.3,1314.1,1316.1,1312.9,1325.7,1323.5,1326.3,1336,1332.1,1330.1,1330.4,1334.7,1341.1,1344.2,1338.8,1348.4,1345.6,1342.8,1334.7,1322.3,1319.3,1314.7,1316.6,1316.4,1315,1325.4,1328.3,1332.2,1329.2,1316.9,1312.3,1309.5,1299.6,1296.9,1277.9,1299.5,1296.2,1298.4,1302.5,1308.7,1306.4,1305.9,1307,1297.2,1301.7,1305,1305.3,1310.2,1307,1308,1319.8,1321.7,1318.7,1316.2,1305.9,1295.8,1293.8,1293.7,1304.2,1302,1285.15,1286.85,1304,1302,1305.2,1323,1344.1,1345.2,1360.1,1355.3,1363.8,1353,1344.7,1353.6,1358,1373.6,1358.2,1369.6,1377.6,1408.9,1425.5,1435.9,1453.7,1438,1426,1439.1,1418,1435,1452.6,1426.65,1437.5,1421.5,1414.1,1433.3,1441.3,1431.4,1433.9,1432.4,1440.8,1462.3,1467,1443.5,1444,1442.9,1447,1437.6,1440.8,1445.7,1447.8,1458.2,1461.9,1481.8,1486.8,1522.7,1521.3,1521.1,1531.5,1546.1,1534.9,1537.7,1538.6,1523.6,1518.8,1518.4,1514.6,1540.3,1565,1554.5,1556.6,1559.8,1541.9,1542.9,1540.05,1558.9,1566.2,1561.9,1536.2,1523.8,1509.1,1506.2,1532.2,1516.6,1519.7,1515,1519.5,1512.1,1524.5,1534.4,1543.3,1543.3,1542.8,1519.5,1507.2,1493.5,1511.4,1525.8,1522.2,1518.8,1515.3,1518,1522.3,1508,1501.5,1503,1495.5,1501.1,1497.9,1498.7,1492.1,1499.4,1506.9,1520.9 };
	const double lows[] = { 1282.85,1315,1318.7,1309.6,1317.6,1312.9,1312.4,1319.1,1319,1321,1318.1,1321.3,1319.9,1312,1280.5,1276.15,1308,1309.9,1308.5,1312.3,1329.3,1333.1,1340.2,1347,1345.9,1338,1340.8,1335,1332,1337.9,1333,1336.8,1333.2,1329.9,1340.4,1323.85,1324.05,1349,1366.3,1351.2,1349.1,1352.4,1350.7,1344.3,1338.9,1316.3,1308.4,1306.9,1309.6,1306.7,1312.3,1315.4,1319,1327.2,1317.2,1320,1323,1328,1323,1327.8,1331.7,1335.3,1336.6,1331.8,1311.4,1310,1309.5,1308,1310.6,1302.8,1306.6,1313.7,1320,1322.8,1311,1312.1,1303.6,1293.9,1293.5,1291,1277.9,1294.1,1286,1289.1,1293.5,1296.9,1298,1299.6,1292.9,1285.1,1288.5,1296.3,1297.2,1298.4,1298.6,1302,1300.3,1312,1310.8,1301.9,1292,1291.1,1286.3,1289.2,1289.9,1297.4,1283.65,1283.25,1292.9,1295.9,1290.8,1304.2,1322.7,1336.1,1341,1343.5,1345.8,1340.3,1335.1,1341.5,1347.6,1352.8,1348.2,1353.7,1356.5,1373.3,1398,1414.7,1427,1416.4,1412.7,1420.1,1396.4,1398.8,1426.6,1412.85,1400.7,1406,1399.8,1404.4,1415.5,1417.2,1421.9,1415,1413.7,1428.1,1434,1435.7,1427.5,1429.4,1423.9,1425.6,1427.5,1434.8,1422.3,1412.1,1442.5,1448.8,1468.2,1484.3,1501.6,1506.2,1498.6,1488.9,1504.5,1518.3,1513.9,1503.3,1503,1506.5,1502.1,1503,1534.8,1535.3,1541.4,1528.6,1525.6,1535.25,1528.15,1528,1542.6,1514.3,1510.7,1505.5,1492.1,1492.9,1496.8,1493.1,1503.4,1500.9,1490.7,1496.3,1505.3,1505.3,1517.9,1507.4,1507.1,1493.3,1470.5,1465,1480.5,1501.7,1501.4,1493.3,1492.1,1505.1,1495.7,1478,1487.1,1480.8,1480.6,1487,1488.3,1484.8,1484,1490.7,1490.4,1503.1 };
	const double closes[] = { 1283.35,1315.3,1326.1,1317.4,1321.5,1317.4,1323.5,1319.2,1321.3,1323.3,1319.7,1325.1,1323.6,1313.8,1282.05,1279.05,1314.2,1315.2,1310.8,1329.1,1334.5,1340.2,1340.5,1350,1347.1,1344.3,1344.6,1339.7,1339.4,1343.7,1337,1338.9,1340.1,1338.7,1346.8,1324.25,1329.55,1369.6,1372.5,1352.4,1357.6,1354.2,1353.4,1346,1341,1323.8,1311.9,1309.1,1312.2,1310.7,1324.3,1315.7,1322.4,1333.8,1319.4,1327.1,1325.8,1330.9,1325.8,1331.6,1336.5,1346.7,1339.2,1334.7,1313.3,1316.5,1312.4,1313.4,1313.3,1312.2,1313.7,1319.9,1326.3,1331.9,1311.3,1313.4,1309.4,1295.2,1294.7,1294.1,1277.9,1295.8,1291.2,1297.4,1297.7,1306.8,1299.4,1303.6,1302.2,1289.9,1299.2,1301.8,1303.6,1299.5,1303.2,1305.3,1319.5,1313.6,1315.1,1303.5,1293,1294.6,1290.4,1291.4,1302.7,1301,1284.15,1284.95,1294.3,1297.9,1304.1,1322.6,1339.3,1340.1,1344.9,1354,1357.4,1340.7,1342.7,1348.2,1355.1,1355.9,1354.2,1362.1,1360.1,1408.3,1411.2,1429.5,1430.1,1426.8,1423.4,1425.1,1400.8,1419.8,1432.9,1423.55,1412.1,1412.2,1412.8,1424.9,1419.3,1424.8,1426.1,1423.6,1435.9,1440.8,1439.4,1439.7,1434.5,1436.5,1427.5,1432.2,1433.3,1441.8,1437.8,1432.4,1457.5,1476.5,1484.2,1519.6,1509.5,1508.5,1517.2,1514.1,1527.8,1531.2,1523.6,1511.6,1515.7,1515.7,1508.5,1537.6,1537.2,1551.8,1549.1,1536.9,1529.4,1538.05,1535.15,1555.9,1560.4,1525.5,1515.5,1511.1,1499.2,1503.2,1507.4,1499.5,1511.5,1513.4,1515.8,1506.2,1515.1,1531.5,1540.2,1512.3,1515.2,1506.4,1472.9,1489,1507.9,1513.8,1512.9,1504.4,1503.9,1512.8,1500.9,1488.7,1497.6,1483.5,1494,1498.3,1494.1,1488.1,1487.5,1495.7,1504.7,1505.3 };

	int NumberPoints = _countof(dates);
	COleDateTime Min((__time64_t)dates[0]);
	COleDateTime Max((__time64_t)dates[NumberPoints - 1]);
	// Create the bottom axis and configure it properly
	m_ChartCtrl.EnableRefresh(false);
	CChartDateTimeAxis* pBottomAxis = m_ChartCtrl.CreateDateTimeAxis(CChartCtrl::BottomAxis);
	pBottomAxis->SetTextColor(RGB(210, 210, 210));
	pBottomAxis->SetMinMax(Min, Max);
	pBottomAxis->SetDiscrete(false);
	pBottomAxis->SetTickIncrement(false, CChartDateTimeAxis::tiDay, 10);
	pBottomAxis->SetTickLabelFormat(false, _T("%m-%d"));

	CChartCandlestickSerie* pSeries = m_ChartCtrl.CreateCandlestickSerie(false, true);
	pSeries->SetWidth(4);
	pSeries->SetName(_T("GOOGL"));
	pSeries->SetColor(RGB(255, 0, 0));// RGB(80, 170, 255));
	pSeries->EnableShadow(false);
	pSeries->SetCandleColor(RGB(255, 50, 53), RGB(80, 170, 255));
	m_ChartCtrl.GetLegend()->UndockLegend(10, 20);
	m_ChartCtrl.GetLegend()->SetHorizontalMode(true);
	m_ChartCtrl.GetLegend()->SetBackColor(RGB(68, 68, 68));
	m_ChartCtrl.GetLegend()->SetTextColor(RGB(255, 255, 255));

	SChartCandlestickPoint ptCandle;
	for (int i = 0; i < NumberPoints; i++) {
		COleDateTime tm((__time64_t)dates[i]);
		ptCandle.XVal = tm;
		ptCandle.Low = lows[i];
		ptCandle.High = highs[i];
		ptCandle.Open = opens[i];
		ptCandle.Close = closes[i];
		pSeries->AddPoint(ptCandle.XVal, ptCandle.Low, ptCandle.High, ptCandle.Open, ptCandle.Close);
	}

	auto* pCrossHair = m_ChartCtrl.CreateCrossHairCursor(false, true);
	pCrossHair->SetColor(RGB(150, 150, 150));

	//m_ChartCtrl.RefreshCtrl();
	m_ChartCtrl.EnableRefresh(true);

	CChartAxis* pAxis = m_ChartCtrl.GetBottomAxis();
	pAxis->SetVisible(true);
	pAxis->GetGrid()->SetVisible(true);
	pAxis->SetAutomatic(true);
	Min = COleDateTime((__time64_t)dates[NumberPoints / 2]);
	pAxis->SetMinMax(Min, Max);
	OnBottomAxisRadio();
	pAxis = m_ChartCtrl.GetRightAxis();
	pAxis->SetVisible(true);
	pAxis->GetGrid()->SetVisible(true);
	pAxis->SetAutomatic(true);
	OnRightAxisRadio();

	int index = m_SeriesList.AddString(pSeries->GetName().c_str());
	m_SeriesList.SetItemData(index, pSeries->GetSerieId());
	OnLegendVisible();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChartDemoDlg::OnOK()
{

	//CDialog::OnOK();
}

void CChartDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChartDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChartDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CChartDemoDlg::OnAddseries() 
{
	CSeriesPropDlg PropertiesDlg;
	if (PropertiesDlg.DoModal() == IDCANCEL)
		return;

	int Type = PropertiesDlg.m_iSeriesType;
	TChartString Name = PropertiesDlg.m_strSeriesName;
	COLORREF Color = PropertiesDlg.m_SeriesColour;

	CChartXYSerie* pSeries = NULL;
	switch (Type)
	{
	// Line series
	case 0:
		{
			bool bSecondHorizAxis = (PropertiesDlg.m_iHorizAxis == 1);
			bool bSecondVertAxis = (PropertiesDlg.m_iVertAxis == 1);
			CChartLineSerie* pLineSeries = m_ChartCtrl.CreateLineSerie(bSecondHorizAxis, bSecondVertAxis);
			pSeries = pLineSeries;

			CLinePropDialog LinePropDlg;
			LinePropDlg.DoModal();

			pLineSeries->SetWidth(LinePropDlg.m_iLineWidth);
			pLineSeries->SetPenStyle(LinePropDlg.m_iPenStyle);
		}
		break;

	// Points series
	case 1:
		{
			bool bSecondHorizAxis = (PropertiesDlg.m_iHorizAxis == 1);
			bool bSecondVertAxis = (PropertiesDlg.m_iVertAxis == 1);
			CChartPointsSerie* pPointsSeries = m_ChartCtrl.CreatePointsSerie(bSecondHorizAxis, bSecondVertAxis);
			pSeries = pPointsSeries;

			CPointsPropDialog PointsPropDlg;
			PointsPropDlg.DoModal();

			switch (PointsPropDlg.m_iPointsType)
			{
			case 0:
				pPointsSeries->SetPointType(CChartPointsSerie::ptEllipse);
				break;

			case 1:
				pPointsSeries->SetPointType(CChartPointsSerie::ptRectangle);
				break;

			case 2:
				pPointsSeries->SetPointType(CChartPointsSerie::ptTriangle);
				break;
			}

			pPointsSeries->SetPointSize(PointsPropDlg.m_iPointsWidth,PointsPropDlg.m_iPointsHeight);
		}
		break;

	// Surface series
	case 2:
		{
			bool bSecondHorizAxis = (PropertiesDlg.m_iHorizAxis == 1);
			bool bSecondVertAxis = (PropertiesDlg.m_iVertAxis == 1);
			CChartSurfaceSerie* pSurfSeries = m_ChartCtrl.CreateSurfaceSerie(bSecondHorizAxis, bSecondVertAxis);
			pSeries = pSurfSeries;

			CSurfacePropDialog SurfPropDlg;
			SurfPropDlg.DoModal();

			switch (SurfPropDlg.m_FillStyle)
			{
			case 0:
				pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsSolid);
				break;
			case 1:
				pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchDownDiag);
				break;
			case 2:
				pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchUpDiag);
				break;
			case 3:
				pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchCross);
				break;
			case 4:
				pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchDiagCross);
				break;
			case 5:
				pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchHorizontal);
				break;
			case 6:
				pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchVertical);
				break;
			}

			if (SurfPropDlg.m_iHorizSurf == 0)
				pSurfSeries->SetHorizontal(true);
			else
				pSurfSeries->SetHorizontal(false);
		}
		break;
	}

	pSeries->SetName(Name);
	pSeries->SetColor(Color);

	int NumberPoints = PropertiesDlg.m_iPointsNumber;
	double* XValues = new double[NumberPoints];
	double* YValues = new double[NumberPoints];
	float Step = (PropertiesDlg.m_fMaxXValue - PropertiesDlg.m_fMinXValue)/(NumberPoints - 1);
	float XStart = PropertiesDlg.m_fMinXValue;
	switch (PropertiesDlg.m_iDataType)
	{
	case 0:
		{
			float Slope = PropertiesDlg.m_fLineSlope;
			float Offset = PropertiesDlg.m_fLineOffset;

			for (int i=0;i<NumberPoints;i++)
			{
				XValues[i] = XStart + i * Step;
				YValues[i] = XValues[i] * Slope + Offset;
			}
			pSeries->SetPoints(XValues,YValues,NumberPoints);

		}
		break;

	case 1:
		{
			float Amplitude = PropertiesDlg.m_fSineAmplitude;
			float Period = PropertiesDlg.m_fSinePeriod;

			for (int i=0;i<NumberPoints;i++)
			{
				XValues[i] = XStart + i * Step;
				YValues[i] = Amplitude * (float)sin( 2 * 3.141592 * XValues[i]/Period );
			}
			pSeries->SetPoints(XValues,YValues,NumberPoints);
		}
		break;

	case 2:
		{
			int Min = PropertiesDlg.m_iRandMinVal;
			int Max = PropertiesDlg.m_iRandMaxVal;
			srand((unsigned int)time(NULL));
			for (int i=0;i<NumberPoints;i++)
			{
				XValues[i] = XStart + i * Step;
				YValues[i] = Min + (rand()%(Max-Min));
			}
			pSeries->SetPoints(XValues,YValues,NumberPoints);
		}
		break;
	}
	
	delete[] XValues;
	delete[] YValues;
	m_ChartCtrl.RefreshCtrl();
	int index = m_SeriesList.AddString(Name.c_str());
	m_SeriesList.SetItemData(index, pSeries->GetSerieId());
}

void CChartDemoDlg::OnLegendVisible() 
{
	if (m_LegendVisBtn.GetCheck() == 1)
		m_ChartCtrl.GetLegend()->SetVisible(true);
	else
		m_ChartCtrl.GetLegend()->SetVisible(false);
	m_ChartCtrl.RefreshCtrl();
}

void CChartDemoDlg::OnBottomAxisRadio() 
{
	CChartAxis* pAxis = m_ChartCtrl.GetBottomAxis();
	if (!pAxis)
		return;
	if (pAxis->IsVisible())
		((CButton*)GetDlgItem(IDC_AXISVISIBLE_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISVISIBLE_CHECK))->SetCheck(0);
	if (pAxis->GetGrid()->IsVisible())
		((CButton*)GetDlgItem(IDC_AXISGRIDVIS_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISGRIDVIS_CHECK))->SetCheck(0);
	if (pAxis->IsAutomatic())
		((CButton*)GetDlgItem(IDC_AXISAUTOMATIC_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISAUTOMATIC_CHECK))->SetCheck(0);
	if (pAxis->IsInverted())
		((CButton*)GetDlgItem(IDC_AXISINVERTED_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISINVERTED_CHECK))->SetCheck(0);
	if (pAxis->ScrollBarEnabled())
		((CButton*)GetDlgItem(IDC_AXISSCROLLBAR_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISSCROLLBAR_CHECK))->SetCheck(0);

	TChartString AxisLabel = pAxis->GetLabel()->GetText();
	GetDlgItem(IDC_AXISLABEL_EDIT)->SetWindowText(AxisLabel.c_str());

	double Min=0, Max=0;
	CString strBuff;
	pAxis->GetMinMax(Min,Max);
	strBuff.Format(_T("%.2f"),Min);
	GetDlgItem(IDC_AXISMINVAL_EDIT)->SetWindowText(strBuff);
	strBuff.Format(_T("%.2f"),Max);
	GetDlgItem(IDC_AXISMAXVAL_EDIT)->SetWindowText(strBuff);
}

void CChartDemoDlg::OnLeftAxisRadio() 
{
	CChartAxis* pAxis = m_ChartCtrl.GetLeftAxis();
	if (!pAxis)
		return;
	if (pAxis->IsVisible())
		((CButton*)GetDlgItem(IDC_AXISVISIBLE_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISVISIBLE_CHECK))->SetCheck(0);
	if (pAxis->GetGrid()->IsVisible())
		((CButton*)GetDlgItem(IDC_AXISGRIDVIS_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISGRIDVIS_CHECK))->SetCheck(0);
	if (pAxis->IsAutomatic())
		((CButton*)GetDlgItem(IDC_AXISAUTOMATIC_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISAUTOMATIC_CHECK))->SetCheck(0);
	if (pAxis->IsInverted())
		((CButton*)GetDlgItem(IDC_AXISINVERTED_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISINVERTED_CHECK))->SetCheck(0);
	if (pAxis->ScrollBarEnabled())
		((CButton*)GetDlgItem(IDC_AXISSCROLLBAR_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISSCROLLBAR_CHECK))->SetCheck(0);

	TChartString AxisLabel = pAxis->GetLabel()->GetText();
	GetDlgItem(IDC_AXISLABEL_EDIT)->SetWindowText(AxisLabel.c_str());

	double Min=0, Max=0;
	CString strBuff;
	pAxis->GetMinMax(Min,Max);
	strBuff.Format(_T("%.2f"),Min);
	GetDlgItem(IDC_AXISMINVAL_EDIT)->SetWindowText(strBuff);
	strBuff.Format(_T("%.2f"),Max);
	GetDlgItem(IDC_AXISMAXVAL_EDIT)->SetWindowText(strBuff);	
}

void CChartDemoDlg::OnRightAxisRadio() 
{
	CChartAxis* pAxis = m_ChartCtrl.GetRightAxis();
	if (!pAxis)
		return;
	if (pAxis->IsVisible())
		((CButton*)GetDlgItem(IDC_AXISVISIBLE_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISVISIBLE_CHECK))->SetCheck(0);
	if (pAxis->GetGrid()->IsVisible())
		((CButton*)GetDlgItem(IDC_AXISGRIDVIS_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISGRIDVIS_CHECK))->SetCheck(0);
	if (pAxis->IsAutomatic())
		((CButton*)GetDlgItem(IDC_AXISAUTOMATIC_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISAUTOMATIC_CHECK))->SetCheck(0);
	if (pAxis->IsInverted())
		((CButton*)GetDlgItem(IDC_AXISINVERTED_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISINVERTED_CHECK))->SetCheck(0);
	if (pAxis->ScrollBarEnabled())
		((CButton*)GetDlgItem(IDC_AXISSCROLLBAR_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISSCROLLBAR_CHECK))->SetCheck(0);

	TChartString AxisLabel = pAxis->GetLabel()->GetText();
	GetDlgItem(IDC_AXISLABEL_EDIT)->SetWindowText(AxisLabel.c_str());

	double Min=0, Max=0;
	CString strBuff;
	pAxis->GetMinMax(Min,Max);
	strBuff.Format(_T("%.2f"),Min);
	GetDlgItem(IDC_AXISMINVAL_EDIT)->SetWindowText(strBuff);
	strBuff.Format(_T("%.2f"),Max);
	GetDlgItem(IDC_AXISMAXVAL_EDIT)->SetWindowText(strBuff);	
}

void CChartDemoDlg::OnTopAxisRadio() 
{
	CChartAxis* pAxis = m_ChartCtrl.GetTopAxis();
	if (!pAxis)
		return;
	if (pAxis->IsVisible())
		((CButton*)GetDlgItem(IDC_AXISVISIBLE_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISVISIBLE_CHECK))->SetCheck(0);
	if (pAxis->GetGrid()->IsVisible())
		((CButton*)GetDlgItem(IDC_AXISGRIDVIS_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISGRIDVIS_CHECK))->SetCheck(0);
	if (pAxis->IsAutomatic())
		((CButton*)GetDlgItem(IDC_AXISAUTOMATIC_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISAUTOMATIC_CHECK))->SetCheck(0);
	if (pAxis->IsInverted())
		((CButton*)GetDlgItem(IDC_AXISINVERTED_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISINVERTED_CHECK))->SetCheck(0);
	if (pAxis->ScrollBarEnabled())
		((CButton*)GetDlgItem(IDC_AXISSCROLLBAR_CHECK))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_AXISSCROLLBAR_CHECK))->SetCheck(0);

	TChartString AxisLabel = pAxis->GetLabel()->GetText();
	GetDlgItem(IDC_AXISLABEL_EDIT)->SetWindowText(AxisLabel.c_str());

	double Min=0, Max=0;
	CString strBuff;
	pAxis->GetMinMax(Min,Max);
	strBuff.Format(_T("%.2f"),Min);
	GetDlgItem(IDC_AXISMINVAL_EDIT)->SetWindowText(strBuff);
	strBuff.Format(_T("%.2f"),Max);
	GetDlgItem(IDC_AXISMAXVAL_EDIT)->SetWindowText(strBuff);	
}

void CChartDemoDlg::OnAxisAutomaticCheck() 
{
	CChartAxis* pAxis = GetSelectedAxis();
	if (!pAxis)
		return;
	if ( ((CButton*)GetDlgItem(IDC_AXISAUTOMATIC_CHECK))->GetCheck() == 1)
		pAxis->SetAutomatic(true);
	else
	{
		TCHAR szBuffer[255];
		double MinVal=0, MaxVal=0;
		m_AxisMinValEdit.GetWindowText(szBuffer,254);
//		MinVal = _tstof(szBuffer);
		MinVal = _tcstod(szBuffer, NULL);
		m_AxisMaxValEdit.GetWindowText(szBuffer,254);
//		MaxVal = _tstof(szBuffer);
		MaxVal = _tcstod(szBuffer, NULL);

		if (MinVal > MaxVal)
		{
			MessageBox(_T("MinVal > MaxVal"),_T("Error"),MB_OK);
			((CButton*)GetDlgItem(IDC_AXISAUTOMATIC_CHECK))->SetCheck(1);
			return;
		}
		pAxis->SetAutomatic(false);
		pAxis->SetMinMax(MinVal,MaxVal);
	}
	m_ChartCtrl.RefreshCtrl();
}

void CChartDemoDlg::OnAxisGridVisCheck() 
{
	CChartAxis* pAxis = GetSelectedAxis();
	if (!pAxis)
		return;
	if ( ((CButton*)GetDlgItem(IDC_AXISGRIDVIS_CHECK))->GetCheck() == 1)
		pAxis->GetGrid()->SetVisible(true);
	else
		pAxis->GetGrid()->SetVisible(false);
	m_ChartCtrl.RefreshCtrl();
}

void CChartDemoDlg::OnAxisVisibleCheck() 
{
	CChartAxis* pAxis = GetSelectedAxis();
	if (!pAxis)
		return;
	if ( ((CButton*)GetDlgItem(IDC_AXISVISIBLE_CHECK))->GetCheck() == 1)
		pAxis->SetVisible(true);
	else
		pAxis->SetVisible(false);	
	m_ChartCtrl.RefreshCtrl();
}

void CChartDemoDlg::OnAxisInvertedCheck() 
{
	CChartAxis* pAxis = GetSelectedAxis();
	if (!pAxis)
		return;
	if ( ((CButton*)GetDlgItem(IDC_AXISINVERTED_CHECK))->GetCheck() == 1)
		pAxis->SetInverted(true);
	else
		pAxis->SetInverted(false);	
	m_ChartCtrl.RefreshCtrl();	
}

void CChartDemoDlg::OnAxisScrollBarCheck()
{
	CChartAxis* pAxis = GetSelectedAxis();
	if (!pAxis)
		return;
	bool bShow = ((CButton*)GetDlgItem(IDC_AXISSCROLLBAR_CHECK))->GetCheck() == 1;
	pAxis->EnableScrollBar(bShow);
}

CChartAxis* CChartDemoDlg::GetSelectedAxis()
{
	if ( ((CButton*)GetDlgItem(IDC_LEFTAXIS_RADIO))->GetCheck() == 1)
		return m_ChartCtrl.GetLeftAxis();
	if ( ((CButton*)GetDlgItem(IDC_BOTTOMAXIS_RADIO))->GetCheck() == 1)
		return m_ChartCtrl.GetBottomAxis();
	if ( ((CButton*)GetDlgItem(IDC_RIGHTAXIS_RADIO))->GetCheck() == 1)
		return m_ChartCtrl.GetRightAxis();
	if ( ((CButton*)GetDlgItem(IDC_TOPAXIS_RADIO))->GetCheck() == 1)
		return m_ChartCtrl.GetTopAxis();

	return NULL;
}

LONG CChartDemoDlg::OnChangeBckCol(UINT , LONG )
{
	COLORREF BackColor = m_BackgndColSel.GetColour();
	m_ChartCtrl.SetBackColor(BackColor);
	m_ChartCtrl.RefreshCtrl();
	return TRUE;
}

void CChartDemoDlg::OnChangeAxisMax() 
{
	CChartAxis* pAxis = GetSelectedAxis();
	TCHAR szBuffer[255];
	double MinVal=0, MaxVal=0;
	m_AxisMinValEdit.GetWindowText(szBuffer,254);
//	MinVal = _tstof(szBuffer);
	MinVal = _tcstod(szBuffer, NULL);
	m_AxisMaxValEdit.GetWindowText(szBuffer,254);
//	MaxVal = _tstof(szBuffer);
	MaxVal = _tcstod(szBuffer, NULL);

	if (MinVal > MaxVal)
	{
		MessageBox(_T("MinVal > MaxVal"),_T("Error"),MB_OK);
		return;
	}
	pAxis->SetMinMax(MinVal,MaxVal);
	
	m_ChartCtrl.RefreshCtrl();
	
}

void CChartDemoDlg::OnChangeAxisMin() 
{
	CChartAxis* pAxis = GetSelectedAxis();
	TCHAR szBuffer[255];
	double MinVal=0, MaxVal=0;
	m_AxisMinValEdit.GetWindowText(szBuffer,254);
//	MinVal = _tstof(szBuffer);
	MinVal = _tcstod(szBuffer, NULL);
	m_AxisMaxValEdit.GetWindowText(szBuffer,254);
//	MaxVal = _tstof(szBuffer);
	MaxVal = _tcstod(szBuffer, NULL);

	if (MinVal > MaxVal)
	{
		MessageBox(_T("MinVal > MaxVal"),_T("Error"),MB_OK);
		return;
	}
	pAxis->SetMinMax(MinVal,MaxVal);
	
	m_ChartCtrl.RefreshCtrl();	
}

void CChartDemoDlg::OnChangeAxisLabel() 
{
	CChartAxis* pAxis = GetSelectedAxis();
	if (!pAxis)
		return;
	TCHAR szBuffer[255];
	GetDlgItem(IDC_AXISLABEL_EDIT)->GetWindowText(szBuffer,254);
	pAxis->GetLabel()->SetText(szBuffer);
	m_ChartCtrl.RefreshCtrl();	
	
}

void CChartDemoDlg::OnDeleteSeries() 
{
	int Index = m_SeriesList.GetCurSel();
	if (Index == -1)
		return;
	unsigned seriesId = m_SeriesList.GetItemData(Index);

	m_ChartCtrl.RemoveSerie(seriesId);
	m_SeriesList.DeleteString(Index);
}

void CChartDemoDlg::OnChangeTitle() 
{
	int Count = m_TitlesEdit.GetLineCount();
	CChartTitle* pTitle = m_ChartCtrl.GetTitle();
	pTitle->RemoveAll();

	TCHAR szBuff[255];
	for (int i=0;i<Count;i++)
	{
		int Size = m_TitlesEdit.GetLine(i,szBuff);
		szBuff[Size] = '\0';
		pTitle->AddString(szBuff);
	}
		
	m_ChartCtrl.RefreshCtrl();	
}

void CChartDemoDlg::OnPanCheck() 
{
	if ( ((CButton*)GetDlgItem(IDC_PAN_CHECK))->GetCheck() == 1)
		m_ChartCtrl.SetPanEnabled(true);
	else
		m_ChartCtrl.SetPanEnabled(false);	
	
}

void CChartDemoDlg::OnZoomCheck() 
{
	if ( ((CButton*)GetDlgItem(IDC_ZOOM_CHECK))->GetCheck() == 1)
		m_ChartCtrl.SetZoomEnabled(true);
	else
		m_ChartCtrl.SetZoomEnabled(false);	
}
