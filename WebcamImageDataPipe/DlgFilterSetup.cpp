#include "PcHeader.h"


#include "Trace.h"
#include "TraceSys.h"
#include "SystemTrace.h"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

#include "WrapperCStd.h"
#include "DlgFilterSetup.h"



using namespace xfp;

DlgFilterSetup::DlgFilterSetup(QWidget *p_Parent, const FILTER& p_Filter)
	: QDialog(p_Parent), m_qlFilterType(this), m_qcbEnable(this),
	m_qpbApply("Apply",this), m_qpbCancel("Cancel",this), m_Filter(p_Filter),
	m_pqhlOptions(NULL), m_pqhlLabel(NULL), m_pqhlValue(NULL)
{
	this->setWindowTitle("Set Filter");
	QVBoxLayout* pqvlMainLayout = new QVBoxLayout(this);
	QHBoxLayout* pqhlFlags = new QHBoxLayout(this);
	QHBoxLayout* pqhlButton = new QHBoxLayout(this);

	// header
	m_qlFilterType.setText(
		QString("< span style = \" font-size:18pt;\">%1</span>")
		.arg(QString::fromStdWString(p_Filter.m_Type)));	

	// flags line
	pqvlMainLayout->addWidget(&m_qlFilterType);
	m_qcbEnable.setText("Enable");
	m_qcbEnable.setChecked(p_Filter.m_Flag & FILTER_FLAG_ENABLE);
	pqhlFlags->addWidget(&m_qcbEnable);
	pqvlMainLayout->addLayout(pqhlFlags);
	if (p_Filter.m_Options.size())
	{
		m_pqhlOptions = new QHBoxLayout(this);
		m_pqhlLabel = new QVBoxLayout(this);
		m_pqhlValue = new QVBoxLayout(this);
		FOR_EACH(it, p_Filter.m_Options)
		{
			QLabel* pql = new QLabel(this);
			QLineEdit* pqle = new QLineEdit(this);
			
			pql->setText(QString::fromStdWString(it->m_Name));
			pqle->setText(QString::fromStdWString(it->m_Value));

			m_pqhlLabel->addWidget(pql);
			m_pqhlValue->addWidget(pqle);
		}

		m_pqhlOptions->addLayout(m_pqhlLabel);
		m_pqhlOptions->addLayout(m_pqhlValue);
		pqvlMainLayout->addLayout(m_pqhlOptions);
	}

	pqvlMainLayout->addStretch();
	// button line
	pqhlButton->addStretch();
	pqhlButton->addWidget(&m_qpbCancel);
	pqhlButton->addWidget(&m_qpbApply);
	pqvlMainLayout->addLayout(pqhlButton);

	//setMinimumWidth(200);
	//setMinimumHeight(50);

	setLayout(pqvlMainLayout);

	// connect
	connect(&m_qpbApply, SIGNAL(pressed()),this,SLOT(OnApply()));
	connect(&m_qpbCancel, SIGNAL(pressed()), this, SLOT(OnCancel()));
}

DlgFilterSetup::~DlgFilterSetup()
{

}

void DlgFilterSetup::OnApply()
{
	done(1);
}

void DlgFilterSetup::OnCancel()
{
	done(0);
}

xfp::FILTER DlgFilterSetup::getFilter()
{
	xfp::FILTER filter(m_Filter);

	if (filter.m_Id.empty())
		filter.m_Id = xfp::getFilterId();

	filter.m_Flag = 0;
	if (m_qcbEnable.isChecked())
		filter.m_Flag |= FILTER_FLAG_ENABLE;

	if (m_pqhlLabel && m_pqhlValue)
	{
		for (int i = 0; i < m_pqhlLabel->count() && i < m_pqhlValue->count(); ++i)
		{
			QLabel* label = (QLabel*) m_pqhlLabel->itemAt(i)->widget();
			QLineEdit* edit = (QLineEdit*) m_pqhlValue->itemAt(i)->widget();

			xfp::setOption(filter.m_Options, label->text().toStdWString(), edit->text().toStdWString());
		}
	}

	return filter;
}
