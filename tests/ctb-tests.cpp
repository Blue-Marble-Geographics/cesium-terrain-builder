//
// Created on 2023/12/02
//

#include "ctb-tests.h"

#include <iostream>
#include <sstream>
#include <string.h>             // for strcmp
#include <stdlib.h>             // for atoi
#include <thread>
#include <mutex>
#include <future>

#include <filesystem>

#include "cpl_multiproc.h"      // for CPLGetNumCPUs
#include "cpl_vsi.h"            // for virtual filesystem
#include "gdal_priv.h"
#include "commander.hpp"        // for cli parsing
#include "concat.hpp"

#include "GlobalMercator.hpp"
#include "RasterIterator.hpp"
#include "TerrainIterator.hpp"
#include "MeshIterator.hpp"
#include "GDALDatasetReader.hpp"
#include "CTBFileTileSerializer.hpp"

namespace fs = std::filesystem;

class CTB_Test : public ::testing::Test {
protected:
  void SetUp() override
  {
    GDALAllRegister();
  } // end of function SetUp
};

template<typename T>
using deleted_unique_ptr = std::unique_ptr<T, std::function<void(T*)>>;

TEST_F(CTB_Test, t0)
{
  const char filename[] = R"(Z:\GM-13206\gardiner_3857.tif)";
  const double VOID_VAL = -32767.0;

  if (!fs::is_regular_file(filename))
    GTEST_SKIP() << "Cannot find the filename: " << filename;

  deleted_unique_ptr< GDALDataset> poDataset(
    (GDALDataset*)GDALOpen(filename, GA_ReadOnly),
    [](auto t) {
      CONSOLE("GDALClose"); GDALClose(t);
    });

  ASSERT_TRUE(poDataset);

  ASSERT_EQ(1, poDataset->GetRasterCount());

  GDALRasterBand* poBand = poDataset->GetRasterBand(1);
  ASSERT_TRUE(poBand != NULL);
  ASSERT_EQ(GDT_Float32, poBand->GetRasterDataType());

  {
    int actual_ok;
    CONSOLE_EVAL(poBand->GetNoDataValue());
    ASSERT_EQ(VOID_VAL, poBand->GetNoDataValue(&actual_ok));
    ASSERT_EQ(1, actual_ok);
  }

  {
    int x, y;
    poBand->GetBlockSize(&x, &y);
    EXPECT_EQ(1405, x);
    EXPECT_EQ(1, y);
  }

  {
    double actual_gt[6] = { 0,0,0,0,0,0 };
    ASSERT_EQ(0, poDataset->GetGeoTransform(actual_gt));
    CONSOLE_FEVAL(actual_gt[0]);
    CONSOLE_FEVAL(actual_gt[1]);
    CONSOLE_FEVAL(actual_gt[2]);
    CONSOLE_FEVAL(actual_gt[3]);
    CONSOLE_FEVAL(actual_gt[4]);
    CONSOLE_FEVAL(actual_gt[5]);
  }

  {
    std::vector<float> pImage(1405 * 1);
    ASSERT_EQ(CE_None, poBand->ReadBlock(0, 0, pImage.data()));
    for (auto i : { 0, 5, 6, 7, 8, 9, 10 })
      CONSOLE("pImage[" << i << "]: " << std::setprecision(20) << pImage[i]);
  }

  {
    std::vector<float> pImage(1405 * 1);
    ASSERT_EQ(CE_None, poBand->ReadBlock(0, 0, pImage.data()));
    for (auto i : { 0, 5, 100, 200, 300, 400, 1404 })
      CONSOLE("pImage[" << i << "]: " << std::setprecision(20) << pImage[i]);

    EXPECT_DOUBLE_EQ(VOID_VAL, pImage[0]);
    EXPECT_DOUBLE_EQ(47.700000762939453, pImage[5]);
    EXPECT_DOUBLE_EQ(56.200000762939453, pImage[100]);
    EXPECT_DOUBLE_EQ(VOID_VAL, pImage[200]);
    EXPECT_DOUBLE_EQ(VOID_VAL, pImage[300]);
    EXPECT_DOUBLE_EQ(VOID_VAL, pImage[400]);
    EXPECT_DOUBLE_EQ(VOID_VAL, pImage[1404]);
  }
  {
    std::vector<float> pImage(1405 * 1);
    ASSERT_EQ(CE_None, poBand->ReadBlock(0, 1547, pImage.data()));
    for (auto i : { 0, 100, 200, 300, 400, 1404 })
      CONSOLE("pImage[" << i << "]: " << std::setprecision(20) << pImage[i]);
    EXPECT_DOUBLE_EQ(40.900001525878906, pImage[0]);
    EXPECT_DOUBLE_EQ(78.5, pImage[100]);
    EXPECT_DOUBLE_EQ(52.299999237060547, pImage[200]);
    EXPECT_DOUBLE_EQ(48.200000762939453, pImage[300]);
    EXPECT_DOUBLE_EQ(44.099998474121094, pImage[400]);
    EXPECT_DOUBLE_EQ(VOID_VAL, pImage[1404]);
  }

  {
    std::vector<float> pImage(1405 * 1);
    ASSERT_EQ(CE_None, poBand->ReadBlock(0, 1581, pImage.data()));
    // The entire block is "invalid" (i.e. empty)
    for (auto x : pImage)
      ASSERT_EQ(VOID_VAL, x);
  }
}

/// @brief Read blocks from gardiner_cropped_ft.tif
/// @param  
/// @param  
TEST_F(CTB_Test, t1)
{
  const char filename[] = R"(Z:\GM-13206\gardiner_cropped_ft.tif)";
  const double VOID_VAL = -32767.0;

  if (!fs::is_regular_file(filename))
    GTEST_SKIP() << "Cannot find the filename: " << filename;

  deleted_unique_ptr< GDALDataset> poDataset(
    (GDALDataset*)GDALOpen(filename, GA_ReadOnly),
    [](auto t) {
      CONSOLE("GDALClose"); GDALClose(t);
    });

  ASSERT_TRUE(poDataset);

  ASSERT_EQ(1, poDataset->GetRasterCount());

  GDALRasterBand* poBand = poDataset->GetRasterBand(1);
  ASSERT_TRUE(poBand != NULL);
  ASSERT_EQ(GDT_Float32, poBand->GetRasterDataType());

  {
    int actual_ok;
    CONSOLE_EVAL(poBand->GetNoDataValue());
    ASSERT_EQ(VOID_VAL, poBand->GetNoDataValue(&actual_ok));
    ASSERT_EQ(1, actual_ok);
  }

  {
    int x, y;
    poBand->GetBlockSize(&x, &y);
    EXPECT_EQ(426, x);
    EXPECT_EQ(1, y);
  }

  {
    double actual_gt[6] = { 0,0,0,0,0,0 };
    ASSERT_EQ(0, poDataset->GetGeoTransform(actual_gt));
    CONSOLE_FEVAL(actual_gt[0]);
    CONSOLE_FEVAL(actual_gt[1]);
    CONSOLE_FEVAL(actual_gt[2]);
    CONSOLE_FEVAL(actual_gt[3]);
    CONSOLE_FEVAL(actual_gt[4]);
    CONSOLE_FEVAL(actual_gt[5]);
  }

  {
    std::vector<float> pImage(426 * 1);
    ASSERT_EQ(CE_None, poBand->ReadBlock(0, 0, pImage.data()));
    for (auto i : { 0, 1, 2, 3, 100, 200, 300, 400, 420 })
      CONSOLE_EVAL(pImage[i]);
  }
  {
    std::vector<float> pImage(426 * 1);
    ASSERT_EQ(CE_None, poBand->ReadBlock(0, 400, pImage.data()));
    for (auto i : { 0, 100, 200, 300, 400, 420 })
      CONSOLE_EVAL(pImage[i]);
  }

  {
      std::vector<float> pImage(426 * 1);
      ASSERT_EQ(CE_Failure, poBand->ReadBlock(0, 527, pImage.data()));
  };
}
