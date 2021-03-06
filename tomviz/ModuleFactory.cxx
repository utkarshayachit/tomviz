/******************************************************************************

  This source file is part of the tomviz project.

  Copyright Kitware, Inc.

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

******************************************************************************/
#include "ModuleFactory.h"

#ifdef DAX_DEVICE_ADAPTER
# include "dax/ModuleStreamingContour.h"
# include "dax/ModuleAccelThreshold.h"
#endif

#include "ModuleContour.h"
#include "ModuleOrthogonalSlice.h"
#include "ModuleOutline.h"
#include "ModuleSegment.h"
#include "ModuleSlice.h"
#include "ModuleThreshold.h"
#include "ModuleVolume.h"
#include "Utilities.h"

#include <pqView.h>
#include <vtkSMViewProxy.h>

#include <QtAlgorithms>

namespace tomviz
{

ModuleFactory::ModuleFactory()
{
}

ModuleFactory::~ModuleFactory()
{
}

QList<QString> ModuleFactory::moduleTypes(
  DataSource* dataSource, vtkSMViewProxy* view)
{
  QList<QString> reply;
  if (dataSource && view)
  {

    // based on the data type and view, return module types.
    reply << "Outline"
      << "Volume"
      << "Contour"
      << "Threshold"
      << "Slice"
      << "Orthogonal Slice";
//      << "Segmentation";
    qSort(reply);
  }
  return reply;
}

Module* ModuleFactory::createModule(
  const QString& type, DataSource* dataSource, vtkSMViewProxy* view)
{
  Module* module = nullptr;
  if (type == "Outline")
  {
    module = new ModuleOutline();
  }
  else if (type == "Contour")
  {
#ifdef DAX_DEVICE_ADAPTER
    module = new ModuleStreamingContour();
#else
    module = new ModuleContour();
#endif
  }
  else if (type == "Volume")
  {
    module = new ModuleVolume();
  }
  else if (type == "Slice")
  {
    module = new ModuleSlice();
  }
  else if (type == "Orthogonal Slice")
  {
    module = new ModuleOrthogonalSlice();
  }
  else if (type == "Threshold")
  {
#ifdef DAX_DEVICE_ADAPTER
    module = new ModuleAccelThreshold();
#else
    module = new ModuleThreshold();
#endif
  }
//  else if (type == "Segmentation")
//  {
//    module = new ModuleSegment();
//  }

  if (module)
  {
    // sanity check.
    Q_ASSERT(type == moduleType(module));
    if (dataSource == nullptr && view == nullptr)
    {
      // don't initialize module if args are NULL.
      return module;
    }

    if (!module->initialize(dataSource, view))
    {
      delete module;
      return nullptr;
    }
    pqView* pqview = tomviz::convert<pqView*>(view);
    pqview->resetDisplay();
    pqview->render();
  }
  return module;
}

QIcon ModuleFactory::moduleIcon(const QString& type)
{
  QIcon icon;
  Module* mdl = ModuleFactory::createModule(type, nullptr, nullptr);
  if (mdl)
  {
    icon = mdl->icon();
    delete mdl;
  }
  return icon;
}

const char* ModuleFactory::moduleType(Module* module)
{
  // WARNING: to ensure the most useful result is returned from this
  // function, the if statements should be ordered so that children
  // classes are checked before parent classes. Otherwise, the module
  // type may be reported to be a class's parent.

  if (qobject_cast<ModuleOutline*>(module))
  {
    return "Outline";
  }
#ifdef DAX_DEVICE_ADAPTER
  if (qobject_cast<ModuleStreamingContour*>(module))
#else
  if (qobject_cast<ModuleContour*>(module))
#endif
  {
    return "Contour";
  }
  if (qobject_cast<ModuleVolume*>(module))
  {
    return "Volume";
  }
  if (qobject_cast<ModuleSlice*>(module))
  {
    return "Slice";
  }
  if (qobject_cast<ModuleOrthogonalSlice*>(module))
  {
    return "Orthogonal Slice";
  }
#ifdef DAX_DEVICE_ADAPTER
  if (qobject_cast<ModuleAccelThreshold*>(module))
#else
  if (qobject_cast<ModuleThreshold*>(module))
#endif
  {
    return "Threshold";
  }
//  if (qobject_cast<ModuleSegment*>(module))
//  {
//    return "Segmentation";
//  }
  return nullptr;
}

} // end of namespace tomviz
