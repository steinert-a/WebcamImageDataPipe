#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>

#include "FilterDefinition.h"
#include "FilterPipe.h"


class DlgFilterSetup : public QDialog
{
	Q_OBJECT

public:
	DlgFilterSetup(QWidget *parent, const xfp::FILTER&);
	~DlgFilterSetup();

private:
	QLabel m_qlFilterType;
	QCheckBox m_qcbEnable;

	QPushButton	m_qpbApply;	
	QPushButton	m_qpbCancel;

	QHBoxLayout* m_pqhlOptions;
	QVBoxLayout* m_pqhlLabel;
	QVBoxLayout* m_pqhlValue;

	xfp::FILTER m_Filter;

public:
	xfp::FILTER getFilter();

private slots:
	void OnApply();
	void OnCancel();
};

