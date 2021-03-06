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
#ifndef tomvizResetReaction_h
#define tomvizResetReaction_h

#include <pqReaction.h>

namespace tomviz
{
class ResetReaction : public pqReaction
{
  Q_OBJECT
  typedef pqReaction Superclass;

public:
  ResetReaction(QAction* action);
  virtual ~ResetReaction();

  static void reset();

protected:
  void onTriggered() override { this->reset(); }

private:
  Q_DISABLE_COPY(ResetReaction)
};

}
#endif
