//
// Copyright (C) 2023 Pablo Delgado Kr�mer
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
//

#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include <cgpu.h>

#include "resizableBuffer.h"

namespace gtl
{
  class GgpuStager;

  class GgpuSyncBuffer
  {
  public:
    enum class UpdateStrategy
    {
      PersistentMapping,
      OptimalStaging
    };

  public:
    GgpuSyncBuffer(CgpuDevice device,
                   GgpuStager& stager,
                   uint64_t elementSize,
                   UpdateStrategy updateStrategy = UpdateStrategy::OptimalStaging,
                   CgpuBufferUsageFlags bufferUsage = 0);

    ~GgpuSyncBuffer();

  public:
    uint8_t* getForReading(uint64_t byteOffset, uint64_t byteSize);

    uint8_t* getForWriting(uint64_t byteOffset, uint64_t byteSize);

    template<typename T>
    T* getForWriting(uint64_t offset, uint64_t range)
    {
      return (T*) getForWriting(offset * m_elementSize, range * m_elementSize);
    }

    template<typename T>
    T* getForReading(uint64_t offset, uint64_t range)
    {
      return (T*) getForReading(offset * m_elementSize, range * m_elementSize);
    }

    bool resize(CgpuDevice device,
                CgpuCommandBuffer commandBuffer,
                uint64_t newSize);

    CgpuBuffer buffer() const;

    uint64_t byteSize() const;

    bool commitChanges(CgpuCommandBuffer commandBuffer);

  private:
    CgpuDevice m_device;
    GgpuStager& m_stager;
    uint64_t m_elementSize;
    UpdateStrategy m_updateStrategy;

    uint64_t m_size = 0;
    GgpuResizableBuffer m_deviceBuffer; // only for OptimalStaging strategy
    GgpuResizableBuffer m_hostBuffer;

    uint8_t* m_mappedHostMem = nullptr;
    uint64_t m_dirtyRangeBegin = 0;
    uint64_t m_dirtyRangeEnd = 0;
  };
}
